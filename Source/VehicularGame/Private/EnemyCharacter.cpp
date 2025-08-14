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
    if(SpeedCurve == nullptr)
    {
        LogError("no speed curve set for the enemies");
        return;
    }


    
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateSpeed(DeltaTime);
    RotateToGround(DeltaTime);
    PathfindToPoint(DeltaTime);
    IncrementTimeSinceLastRammed(DeltaTime);

    if(bIsOverlappingWithPlayer)
    {
        HitByPlayer();
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
    if(SpeedCurve == nullptr)
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
        float SpeedCurveOutput = SpeedCurve->GetFloatValue(SpeedKMH);
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
    FRotator TargetRotation(OutSlopePitchDegreeAngle, GetMesh()->GetComponentRotation().Yaw, OutSlopeRollDegreeAngle * 1.0f);
    GetMesh()->SetWorldRotation(UKismetMathLibrary::RInterpTo(GetMesh()->GetComponentRotation(), TargetRotation, DeltaTime, 2.0f));
    
}

void AEnemyCharacter::PathfindToPoint(float DeltaTime)
{
    if(VehicleRef == nullptr)
    {
        LogError("failed to get a reference to the player in enemy character");
        return;
    }
    
    TimeSinceLastNavUpdate += DeltaTime;
    
    //is it time to update?
    if(TimeSinceLastNavUpdate > (GetDistanceTo(VehicleRef) * NavUpdateDistanceScaleFactor) / 10000.0f)
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
        FVector ProjectionPoint(XInterp, YInterp, ZInterp);

        //FVec
        NavSystem->ProjectPointToNavigation(ProjectionPoint);

        
    }
}

void AEnemyCharacter::IncrementTimeSinceLastRammed(float DeltaTime)
{
    
}

void AEnemyCharacter::HitByPlayer()
{
    
}




