// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "VehicularGameState.h"
#include "TurretRotationComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void ATurret::LogError(const FString& ErrorMessage)
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

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup default subobjects
	TurretBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Base Mesh"));
	TurretRotationComponent = CreateDefaultSubobject<UTurretRotationComponent>(TEXT("Turret Rotation Component"));
	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Head Mesh"));
	BulletSpawnL = CreateDefaultSubobject<USceneComponent>(TEXT("Left Bullet Spawn Location"));
	BulletSpawnR = CreateDefaultSubobject<USceneComponent>(TEXT("Right Bullet Spawn Location"));
	
	//setup attachments
	TurretBaseMesh->SetupAttachment(RootComponent);
	TurretRotationComponent->SetupAttachment(TurretBaseMesh);
	TurretHeadMesh->SetupAttachment(TurretRotationComponent);
	BulletSpawnL->SetupAttachment(TurretHeadMesh);
	BulletSpawnR->SetupAttachment(TurretHeadMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	//save a reference to the game state
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if(GameStateBase == nullptr)
	{
		LogError("No game state found in the turret");
		return;
	}
	VehicularGameState = Cast<AVehicularGameState>(GameStateBase);
	if(VehicularGameState == nullptr)
	{
		LogError("Failed to cast game state to vehicular game state in turret");
		return;
	}

	if(BulletBlueprint == nullptr)
	{
		LogError("Bullet Blueprint is not set in the turret");
		return;
	}

	if(BulletSoundFile == nullptr)
	{
		LogError("Bullet Sound is not set in the turret");
		return;
	}

	BulletSoundInstance = UGameplayStatics::CreateSound2D(this, BulletSoundFile,
	1.0f, 1.0f, 0.0, nullptr,
	false, false);
	if(BulletSoundInstance == nullptr)
	{
		LogError("failed to create an instance of the bullet sound");
		return;
	}

	//prevents the bullet sound from being garbage collected
	//BulletSoundInstance->AddToRoot();
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//make sure we have a turret rotation component
	if(TurretRotationComponent == nullptr)
	{
		LogError("failed to access turret rotation component in turret");
		return;
	}

	//tell the turret to rotate
	TurretRotationComponent->RotateTurretHead(VehicleCamera->GetComponentTransform(), DeltaTime, 10000, 20);

	//check if the time since last shot timer should still increment
	if (TimeSinceLastShot < (1.0f / UpgradedFireRate()))
	{
		//increment time since last shot
		TimeSinceLastShot += DeltaTime;
	}

}

void ATurret::InitializeVariables(UCameraComponent* InCameraComponent, APawn* InShootingPawn)
{
	if(InCameraComponent == nullptr)
	{
		LogError("camera component in the turret initialise variables is null");
		return;
	}

	if(InShootingPawn == nullptr)
	{
		LogError("pawn in the turret initialise variables is null");
		return;
	}
	
	VehicleCamera = InCameraComponent;
	ShooterPawn = InShootingPawn;
}

//no function 
void ATurret::FirePressed()
{
	
}

//checks if we can shoot, and shoots
void ATurret::FireHeld()
{
	if (VehicularGameState == nullptr)
	{
		LogError("failed to get game state in fire held in turret");
		return;
	}

	//can we shoot?
	if (TimeSinceLastShot > (1.0f / UpgradedFireRate()))
	{
		float Spread = VehicularGameState->GetProjectileSpreadMultiplier() * MaxSpreadAngle;

		//setup Instigator 
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = ShooterPawn;

		//are we shooting left or right barrel
		if (bFireFromLeftBarrel)
		{
			const FVector Location = BulletSpawnL->GetComponentLocation();
			const FRotator Rotation = BulletSpawnL->GetComponentRotation();
			GetWorld()->SpawnActor(BulletBlueprint, &Location, &Rotation, SpawnParams); 
		}
		else
		{
			const FVector Location = BulletSpawnR->GetComponentLocation();
			const FRotator Rotation = BulletSpawnR->GetComponentRotation();
			GetWorld()->SpawnActor(BulletBlueprint, &Location, &Rotation, SpawnParams); 
		}

		//reset time since last shot
		TimeSinceLastShot = 0.0f;
		
		//flip barrel
		bFireFromLeftBarrel = !bFireFromLeftBarrel;

		//play the sound
		if(BulletSoundInstance == nullptr)
		{
			if(BulletSoundFile == nullptr)
			{
				LogError("nevermind, there is no bullet audio file. give the turret a sound to play");
				return;
			}
			BulletSoundInstance = UGameplayStatics::CreateSound2D(this, BulletSoundFile,
			1.0f, 1.0f, 0.0, nullptr,
	false, false);
		}
		BulletSoundInstance->Play();
	}
}

//no function
void ATurret::FireReleased()
{
	
}


float ATurret::UpgradedFireRate()
{
	//do we have the game state reference?
	if (VehicularGameState == nullptr)
	{
		LogError("failed to get vehicular game state in the turret");
		return 0.0f;
	}

	//set the upgraded fire rate to what our base fire rate is
	float UpgradedFireRate = FireRate;

	//then add on the upgrade
	switch (VehicularGameState->GetFireRateUpgradeLevel())
	{
		case 0:
			UpgradedFireRate += 0.0f;
			break;
	 	case 1:
	 		UpgradedFireRate += 2.0f;
	 		break;
	
	 	case 2:
	 		UpgradedFireRate += 4.0f;
	 		break;
	
	 	case 3:
	 		UpgradedFireRate += 8.0f;
	 		break;
	
	 	default:
	 		LogError("Unknown fire rate upgrade level in turret");
	 		break;
	 }

	return UpgradedFireRate;

}

//returns a transform with a random amount of spread
FRotator ATurret::GetRotationWithSpread(const FTransform& InputTransform, const float SpreadAngle) const
{
	//get the rotator from the spawn transform
	FRotator MyRotator = InputTransform.Rotator();

	//get randomised directions
	float RandomPitch = UKismetMathLibrary::RandomFloatInRange(SpreadAngle * 1.0f, SpreadAngle);
	float RandomYaw = UKismetMathLibrary::RandomFloatInRange(SpreadAngle * 1.0f, SpreadAngle);

	//make a rotator from these values
	FRotator RandomRotation(RandomPitch, RandomYaw, 0.0f);

	//add the random rotation to the spawn rotation
	MyRotator += RandomRotation;

	//return the new rotation
	return MyRotator;
}
