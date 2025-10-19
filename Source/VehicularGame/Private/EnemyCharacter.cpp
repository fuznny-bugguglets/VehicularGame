#include "EnemyCharacter.h"
#include "AIController.h" // For AAIController
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "Vehicle.h"
#include "VehicularGameState.h"
#include "GameFramework/CharacterMovementComponent.h" // For character movement
#include "Kismet/GameplayStatics.h" // For getting player character
#include "Components/CapsuleComponent.h" // For UCapsuleComponent
#include "Kismet/KismetMathLibrary.h"

void AEnemyCharacter::LogError(const FString& ErrorMessage)
{
    //if we have the engine pointer, we print to the screen
    if(GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorMessage);
    }
    //otherwise, we print to the log
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorMessage);
    }
}


AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true; // Allows this actor to tick
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character rotates in the direction of input
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // Rotation speed
    GetCharacterMovement()->MaxWalkSpeed = 300.f; // Walking speed
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth; // Set current health to max health at start

    //get the game state (vehicular)
    if(AGameStateBase* GameStateBase = GetWorld()->GetGameState())
    {
        VehicularGameState = Cast<AVehicularGameState>(GameStateBase);
        if(!VehicularGameState)
        {
            LogError("couldn't get the vehicular game state in enemy character");
            return;
        }
    }
    else
    {
        LogError("couldn't get the game state in enemy character");
        return;
    }

    //get the player vehicle
    if(APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        VehicleRef = Cast<AVehicle>(MyPawn);
        if(VehicleRef == nullptr)
        {
            LogError("failed to cast the player pawn to a vehicle in enemy");
            return;
        }
    }
    else
    {
        LogError("failed to get a pawn in enemy character");
        return;
    }

    //check the float curve is active
    if(MySpeedCurve == nullptr)
    {
        LogError("no speed curve set for the enemies");
        return;
    }

    if(AttackPlayerSound == nullptr)
    {
        LogError("no attack player sound set in enemy");
        return;
    }

    if (AttackPlayerConcurrency == nullptr)
    {
        LogError("no attack player concurrency set in enemy");
        return;
    }

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlap);
    GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnHit);


    
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);


    if (VehicleRef == nullptr)
    {
        LogError("failed to get a reference to the player in enemy character");
        return;
    }

    //declares for the switch statement
    float DistanceToPlayer = 0.0f;
    FVector NewLocation = FVector::Zero();
    FVector TargetLocation = FVector::Zero();
    AAIController* AIController = nullptr;

    switch (EnemyState)
    {
    case EEnemyState::RUNNING:
        UpdateSpeed(DeltaTime);
        RotateToGround(DeltaTime);

        //is it time to update?
        TimeSinceLastNavUpdate += DeltaTime;
        if (TimeSinceLastNavUpdate > (GetDistanceTo(VehicleRef) * NavUpdateDistanceScaleFactor) / 10000.0f)
        {
            PathfindToPoint();
        }

        //is it time to lunge?
        ElapsedLungeCooldownTime += DeltaTime;
        if (ElapsedLungeCooldownTime > LungeCooldown)
        {
            //check distance from player
            DistanceToPlayer = FVector::DistSquared(GetActorLocation(), VehicleRef->GetActorLocation());
            //is it close enough to lunge?
            if (DistanceToPlayer < LungeDistance * LungeDistance)
            {
                //lunge
                EnemyState = EEnemyState::LUNGING;

                //reset elapsed time
                ElapsedLungeTime = 0.0f;

                //direction amount in front of the player
                TargetLocation = VehicleRef->GetActorForwardVector() * VehicleRef->GetSpeed();
                TargetLocation *= FMath::RandRange(MinLungeForwardPredictionFactor, MaxLungeForwardPredictionFactor);

                //add it to the player location
                TargetLocation = VehicleRef->GetActorLocation() + TargetLocation;

                //calculate the direction for the enemy to lunge in
                LungeDirection = TargetLocation - GetActorLocation();
                LungeDirection.Normalize();
            }
        }

        

        break;

    case EEnemyState::LUNGING:
        // Stop AI Controller
        AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            AIController->StopMovement();
        }

        //increment time
        ElapsedLungeTime += DeltaTime;

        //have we exceeded our time?
        if (ElapsedLungeTime > LungeTime)
        {
            //finish the lunge
            EnemyState = EEnemyState::ATTACKING;
        }

        //move toward player
    	NewLocation = GetActorLocation() + (LungeDirection * LungeSpeed * DeltaTime);
        SetActorLocation(NewLocation);
        break;

    case EEnemyState::ATTACKING:

        //just to go back to running
    	EnemyState = EEnemyState::RUNNING;

        //find a new path
        PathfindToPoint();

        //reset elapsed time
        ElapsedLungeCooldownTime = 0.0f;
    	break;

    default:
        break;

    }

}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage > 0.f && !IsDead())
    {
        CurrentHealth -= ActualDamage;

        if (CurrentHealth <= 0.f)
        {
            Die();
        }
    }
    return ActualDamage;
}


void AEnemyCharacter::Die()
{
    // Cleanup before destruction
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();

    // Disable all collision on the capsule
    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    if (CapsuleComp)
    {
        CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    }

    // Stop AI Controller
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->StopMovement();
    }

    // Destroy the enemy.
    Destroy();
}

bool AEnemyCharacter::IsDead() const
{
    return CurrentHealth <= 0.f;
}

void AEnemyCharacter::UpdateSpeed(float DeltaTime)
{
    if(MySpeedCurve == nullptr)
    {
        LogError("no speed curve set for the enemies");
        return;
    }

    if(VehicleRef == nullptr)
    {
        LogError("failed to access player in enemy");
        return;
    }

    

    AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
    if(EnemyAIController == nullptr)
    {
        LogError("failed to get the enemy ai controller from enemy character");
        return;
    }

    //are we close to the player?
    if(FVector::Dist(GetActorLocation(), EnemyAIController->GetMovingTargetLocation()) < EnemyAIController->GetTargetLineForwardOffset())
    {
        //reduce target speed (slow down as we approach the player)
        TargetSpeed -= DeltaTime * CloseToPlayerDeceleration;
    }
    else
    {
        //set target speed based on float curve
        float SpeedKMH = VehicleRef->GetSpeed() * 0.3666f;
        float SpeedCurveOutput = MySpeedCurve->GetFloatValue(SpeedKMH);
        TargetSpeed = SpeedCurveOutput + SpeedKMH;
    }

    float ClampResult = 0.0f;
    //is our average target speed greater than our current target speed?
    if(RollingAverageTargetSpeed > TargetSpeed)
    {
        //update the average
        RollingAverageTargetSpeed += SpeedChangeRatePerSec * DeltaTime;

        //clamp it
        ClampResult = UKismetMathLibrary::Clamp(RollingAverageTargetSpeed, RollingAverageTargetSpeed, TargetSpeed);
    }
    else
    {
        //update the average
        RollingAverageTargetSpeed += SpeedChangeRatePerSec * DeltaTime * -1.0f;

        //clamp it
        ClampResult = UKismetMathLibrary::Clamp(RollingAverageTargetSpeed, TargetSpeed, RollingAverageTargetSpeed);
    }

    if(GetCharacterMovement() == nullptr)
    {
        LogError("failed to get character movement in enemy");
    }

    //update move speed (transfer from kmh to cms)
    GetCharacterMovement()->MaxWalkSpeed = ClampResult * 27.2777f;
    
}

void AEnemyCharacter::RotateToGround(float DeltaTime)
{
    //raycast down
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        GetActorLocation(),
         GetActorLocation() - (GetActorUpVector() * 1000),
         ECC_Visibility
         );

    //figure out rotation to be put nicely against the ground
    float OutSlopePitchDegreeAngle = 0.0f;
    float OutSlopeRollDegreeAngle = 0.0f;
    UKismetMathLibrary::GetSlopeDegreeAngles(GetActorRightVector(), HitResult.ImpactNormal, GetActorUpVector(),
        OutSlopePitchDegreeAngle, OutSlopeRollDegreeAngle);

    //interpolate between current and target rotation
    FRotator TargetRotation(OutSlopePitchDegreeAngle, GetMesh()->GetComponentRotation().Yaw, OutSlopeRollDegreeAngle * -1.0f);
    GetMesh()->SetWorldRotation(UKismetMathLibrary::RInterpTo(GetMesh()->GetComponentRotation(), TargetRotation, DeltaTime, 2.0f));
    
}

void AEnemyCharacter::PathfindToPoint()
{
    TimeSinceLastNavUpdate = 0.0f;

        if(IsDead())
        {
            return;
        }

        //grab the ai controller
        AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
        if (AIController == nullptr)
        {
            LogError("failed to get ai controller for enemy");
            return;
        }

        float Alpha = VehicleRef->GetSpeed() / (PlayerMinSpeed * 27.77f);
        if(GetDistanceTo(VehicleRef) > 3000.0f)
        {
            Alpha = 1.0f;
        }
        else
        {
            Alpha = UKismetMathLibrary::FClamp(Alpha, 0.0f, 1.0f);
        }
        
        //interp points
        float XInterp = UKismetMathLibrary::FInterpEaseInOut(
            VehicleRef->GetActorLocation().X,
            AIController->GetMovingTargetLocation().X,
            Alpha,
            5.0f);
        float YInterp = UKismetMathLibrary::FInterpEaseInOut(
            VehicleRef->GetActorLocation().Y,
            AIController->GetMovingTargetLocation().Y,
            Alpha,
            5.0f);
        float ZInterp = UKismetMathLibrary::FInterpEaseInOut(
            VehicleRef->GetActorLocation().Z,
            AIController->GetMovingTargetLocation().Z,
            Alpha,
            5.0f);
        
        UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
        if(NavSystem == nullptr)
        {
            LogError("failed to get the nav system in character");
            return;
        }

        //create a point from the interped values
        const FVector InterpProjectionPoint(XInterp, YInterp, ZInterp);

        FNavLocation OutLocation;
        const FVector Extent(5000.f);

        //honestly no idea. john did this
        //pretty sure it tries to go to some interpolated point
        if(NavSystem->ProjectPointToNavigation(InterpProjectionPoint, OutLocation, Extent))
        {
            AIController->MoveToLocation(OutLocation, 1.0f);
        }
        //otherwise it tries to go to the player (on the nav mesh)
        else if(NavSystem->ProjectPointToNavigation(VehicleRef->GetActorLocation(), OutLocation, Extent))
        {
            AIController->MoveToLocation(OutLocation, 1.0f);
        }
        //and if all else fails, fuck it, move as close as you can to the player
        else
        {
            AIController->MoveToActor(VehicleRef, 1.0f);
        }

        
        

        
}

void AEnemyCharacter::HitByPlayer()
{
    if(VehicleRef == nullptr)
    {
        LogError("failed to access player reference in enemy");
        return;
    }

    if(VehicleRef->GetStaticMesh() == nullptr)
    {
        LogError("failed to access static mesh component from player in enemy");
        return;
    }

    if(GetCharacterMovement() == nullptr)
    {
        LogError("failed to get character movement component in enemy");
    }

    FVector PlayerMeshVelocity = VehicleRef->GetStaticMesh()->GetPhysicsLinearVelocity();

    GetCharacterMovement()->SetMovementMode(MOVE_Falling);

    FVector PushVector = UKismetMathLibrary::GetDirectionUnitVector(VehicleRef->GetActorLocation(), GetActorLocation());
    PushVector.Z = 0.5f;
    PushVector *= PlayerPushForce;
    

    //is the enemy in front of the vehicle?
    if(UKismetMathLibrary::InverseTransformLocation(VehicleRef->GetActorTransform(), GetActorLocation()).X > 250.0f)
    {
        //launch ourselves away from the vehicle with extra oomf
        LaunchCharacter(PushVector * 2.5f, false, false);
    }

    //deal damage to the player
    else
    {
        const TSubclassOf<UDamageType> DamageType;
        //deal damage to the player
        UGameplayStatics::ApplyDamage(VehicleRef, DamageToPlayer, GetController(), this, DamageType);

        //play attack damage sound
        UGameplayStatics::PlaySound2D(this, AttackPlayerSound, 1, 1,
            0, AttackPlayerConcurrency);
        
        //launch ourselves away from the vehicle
        LaunchCharacter(PushVector, false, false);
    }

    //finish the lunge
    EnemyState = EEnemyState::ATTACKING;
    
     
}

//old code that used to use collisions. now uses overlaps
void AEnemyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //is it the vehicle
    if(Cast<AVehicle>(OtherActor))
    {
        HitByPlayer();
    }
    
}

void AEnemyCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //is it the vehicle
    if(Cast<AVehicle>(OtherActor))
    {
        HitByPlayer();
    }
}


void AEnemyCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    //when we land on the ground, pathfind to point
    PathfindToPoint();
}

EEnemyState AEnemyCharacter::GetEnemyState() const
{
    return EnemyState;
}
