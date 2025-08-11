// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "VehicularGameState.h"
#include "TurretRotationComponent.h"
#include "Camera/CameraComponent.h"

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
	
	
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TurretRotationComponent == nullptr)
	{
		LogError("failed to access turret rotation component in turret");
		return;
	}

	TurretRotationComponent->RotateTurretHead(VehicleCamera->GetComponentTransform(), DeltaTime, 10000, 20);

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

void ATurret::FirePressed()
{
	
}

void ATurret::FireHeld()
{
	
}

void ATurret::FireReleased()
{
	
}




