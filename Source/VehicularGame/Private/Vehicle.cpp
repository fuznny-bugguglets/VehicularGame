// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CustomWheelComponent.h"
#include "GSScavenger.h"
#include "Turret.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "WheelFrictionCurve.h"

//handy shortcut to displaying things when shit goes wrong
void AVehicle::LogError(const FString& ErrorMessage)
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
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setup components and attachment
	UStaticMeshComponent* VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vehicle Mesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TurretChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret"));
	FrontLeftWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Front Left Wheel"));
	FrontRightWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Front Right Wheel"));
	BackLeftWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Back Left Wheel"));
	BackRightWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Back Right Wheel"));
	UStaticMeshComponent* FrontLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front Left Wheel Mesh"));
	UStaticMeshComponent* FrontRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front Right Wheel Mesh"));
	UStaticMeshComponent* BackLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Left Wheel Mesh"));
	UStaticMeshComponent* BackRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Right Wheel Mesh"));
	VehicleMesh->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(VehicleMesh);
	Camera->SetupAttachment(SpringArm);
	TurretChildActorComponent->SetupAttachment(VehicleMesh);
	FrontLeftWheel->SetupAttachment(VehicleMesh);
	FrontRightWheel->SetupAttachment(VehicleMesh);
	BackLeftWheel->SetupAttachment(VehicleMesh);
	BackRightWheel->SetupAttachment(VehicleMesh);
	FrontLeftWheelMesh->SetupAttachment(FrontLeftWheel);
	FrontRightWheelMesh->SetupAttachment(FrontRightWheel);
	BackLeftWheelMesh->SetupAttachment(BackLeftWheel);
	BackRightWheelMesh->SetupAttachment(BackRightWheel);

	//binds damage delegate
	OnTakeAnyDamage.AddDynamic(this, &AVehicle::OnTakeDamage);
	

}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
	//get the game state
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if(GameStateBase == nullptr)
	{
		LogError(TEXT("Failed to get the game state base in vehicle"));
		return;
	}

	//get the scavenger game state
	ScavengerGameState = Cast<AGSScavenger>(GameStateBase);
	if(ScavengerGameState == nullptr)
	{
		LogError(TEXT("Failed to get the Scavenger Game State from Vehicle"));
		return;
	}

	//creates the sound
	if(EngineSound == nullptr)
	{
		LogError(TEXT("No Engine Sound set in Vehicle"));
		return;
	}
	EngineSoundInstance = UGameplayStatics::CreateSound2D(this, EngineSound);

	//sets the volume of the sound
	SetEngineSoundValues();

	//begins playing
	EngineSoundInstance->Play();

	//sets the child actor to be a turret
	TurretChildActorComponent->SetChildActorClass(TurretClass);

	//get the turret from the child actor
	Turret = Cast<ATurret>(TurretChildActorComponent->GetChildActor());
	if(Turret == nullptr)
	{
		LogError("Failed to access the turret class from the vehicle's turret child");
		return;
	}

	//initialize the turret
	Turret->InitializeVariables(Camera, this);

	//set our health to our max health
	Health = MaxHealth;
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			LogError("Failed to get input subsystem in vehicle");
		}
	}
	else
	{
		LogError("Failed to get player controller in vehicle");
	}

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//bind the inputs
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVehicle::OnLook);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVehicle::OnMove);
		EnhancedInputComponent->BindAction(DriftAction, ETriggerEvent::Started, this, &AVehicle::OnStartDrift);
		EnhancedInputComponent->BindAction(DriftAction, ETriggerEvent::Completed, this, &AVehicle::OnStopDrift);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AVehicle::OnHandbreak);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AVehicle::OnFireStart);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Ongoing, this, &AVehicle::OnFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AVehicle::OnFireStop);
		EnhancedInputComponent->BindAction(EngineShiftUpAction, ETriggerEvent::Triggered, this, &AVehicle::OnEngineShiftUp);
		EnhancedInputComponent->BindAction(EngineShiftDownAction, ETriggerEvent::Triggered, this, &AVehicle::OnEngineShiftDown);
	}
	else
	{
		LogError("Failed to find an enhanced input component in vehicle");
	}

}

// Loot
int32 AVehicle::GetCommonLootCount() const
{
	return CommonLootCount;
}
int32 AVehicle::GetUncommonLootCount() const
{
	return UncommonLootCount;
}
int32 AVehicle::GetRareLootCount() const
{
	return RareLootCount;
}

void AVehicle::ResetAllLoot()
{
	SetCommonLootCount(0);
	SetUncommonLootCount(0);
	SetRareLootCount(0);
} 
void AVehicle::SetCommonLootCount(int32 NewValue)
{
	CommonLootCount = NewValue;
}
void AVehicle::SetUncommonLootCount(int32 NewValue)
{
	UncommonLootCount = NewValue;
}
void AVehicle::SetRareLootCount(int32 NewValue)
{
	RareLootCount = NewValue;
}

//sets the speed and pitch of the engine based on speed
void AVehicle::SetEngineSoundValues()
{
	//check that the sound instance is in the world and kicking
	if(EngineSoundInstance == nullptr)
	{
		LogError("Trying to set the parameters of the engine sound instance when it doesnt exist in Vehicle");
		return;
	}

	//set new volume
	float NewVolume = (((EngineMaxVolume - EngineMinVolume) / SpeedRequiredForMaxEngineSound) * (Speed)) + EngineMinVolume;
	EngineSoundInstance->SetVolumeMultiplier(NewVolume);

	//set new pitch
	float NewPitch = (((EngineMaxPitch - EngineMinPitch) / SpeedRequiredForMaxEngineSound) * (Speed)) + EngineMinPitch;
	EngineSoundInstance->SetPitchMultiplier(NewPitch);
}

//when the player looks around
void AVehicle::OnLook(const FInputActionValue& Value)
{
	//get the vec2 out of the input
	FVector2D InputVector = Value.Get<FVector2D>();
	
	//check we have a spring arm
	if(SpringArm == nullptr)
	{
		LogError("failed to find spring arm in OnLook in Vehicle");
		return;
	}

	//left and right rotation
	FRotator LeftRightRotation(0.0f, InputVector.X, 0.0f);
	SpringArm->AddRelativeRotation(LeftRightRotation);

	//up and down rotation
	float FlippedY = InputVector.Y * -1.0f;
	FRotator UpDownRotation(FlippedY, 0.0f, 0.0f);
	SpringArm->AddLocalRotation(UpDownRotation);
}

//when the player moves
void AVehicle::OnMove(const FInputActionValue& Value)
{
	//get the vec2 out of the input
	FVector2D InputVector = Value.Get<FVector2D>();
	
	//what are max speed can be, taking in default plus upgrades
	float MyMaxTorque = ScavengerGameState->GetAdditionalMaxSpeed() + MaxTorqueSpeed;

	//multiplier for our speed increase (based on engine state)
	float EngineStateMultiplier = 0.0f;
	switch (CurrentEngineState)
	{
	case EEngineState::OFF:
		EngineStateMultiplier = 0.0f;
		break;

	case EEngineState::CRUISE:
		EngineStateMultiplier = 1.0f;
		break;

	case EEngineState::BOOST1:
		EngineStateMultiplier = 1.4f;
		break;

	case EEngineState::BOOST2:
		EngineStateMultiplier = 1.75f;
		break;
	}

	//how fast we could possibly go
	float MyMaxSpeed = MyMaxTorque * EngineStateMultiplier;

	//input for the torque curve, scaled between current speed and max
	float TorqueCurveInput = Speed / MyMaxSpeed;

	//get the torque based on our input
	float TorqueCurveOutput = TorqueCurve->GetFloatValue(TorqueCurveInput);

	//get the amount of torque to apply (before the engine state scaling)
	float TorqueToApply = TorqueCurveOutput * MaxMotorTorque;

	//scale the torque by the engine state
	TorqueToApply *= EngineStateMultiplier;

	//scale the torque by our input (1 for W, -1 for S, 0 for nothing)
	TorqueToApply *= InputVector.Y;

	//set the motor torque for the wheels
	FrontLeftWheel->SetMotorTorque(TorqueToApply);
	FrontRightWheel->SetMotorTorque(TorqueToApply);
	BackLeftWheel->SetMotorTorque(TorqueToApply);
	BackRightWheel->SetMotorTorque(TorqueToApply);

	//a value that represents our progress to max speed (0 - 1)
	float NormalizedCurrentSpeed = Speed / MaxSteerSpeed;

	//clamp between 0 and 1
	NormalizedCurrentSpeed = FMath::Clamp(NormalizedCurrentSpeed, 0.0f, 1.0f);

	//get the new current steer angle
	float CurrentSteerAngle = FMath::Lerp(MaxSteerAngle, MinSteerAngle, NormalizedCurrentSpeed);
	CurrentSteerAngle *= InputVector.X;
	TargetSteerAngle = CurrentSteerAngle;
}

//when the player drifts
void AVehicle::OnStartDrift(const FInputActionValue& Value)
{
	//make sure the handbrake is off
	if(bHandbrakeActive)
	{
		return;
	}

	//change the stiffness of the wheels
	FrontLeftWheel->SidewaysFrictionCurve.Stiffness = DriftSidewaysStiffness;
	FrontRightWheel->SidewaysFrictionCurve.Stiffness = DriftSidewaysStiffness;
	BackLeftWheel->SidewaysFrictionCurve.Stiffness = DriftSidewaysStiffness;
	BackRightWheel->SidewaysFrictionCurve.Stiffness = DriftSidewaysStiffness;

	//update the wheel info
	FrontLeftWheel->SidewaysFrictionCurve.UpdateArrays();
	FrontRightWheel->SidewaysFrictionCurve.UpdateArrays();
	BackLeftWheel->SidewaysFrictionCurve.UpdateArrays();
	BackRightWheel->SidewaysFrictionCurve.UpdateArrays();
}

//when the player stops drifts
void AVehicle::OnStopDrift(const FInputActionValue& Value)
{
	//change the stiffness of the wheels
	FrontLeftWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;
	FrontRightWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;
	BackLeftWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;
	BackRightWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;

	//update the wheel info
	FrontLeftWheel->SidewaysFrictionCurve.UpdateArrays();
	FrontRightWheel->SidewaysFrictionCurve.UpdateArrays();
	BackLeftWheel->SidewaysFrictionCurve.UpdateArrays();
	BackRightWheel->SidewaysFrictionCurve.UpdateArrays();
}

//when the player uses the handbrake
void AVehicle::OnHandbreak(const FInputActionValue& Value)
{
	//checks whether the handbrake is active or inactive
	if(bHandbrakeActive)
	{
		//makes the handbrake off
		bHandbrakeActive = false;

		//set the brake torque to 0 for all wheels
		FrontLeftWheel->SetBrakeTorque(0);
		FrontRightWheel->SetBrakeTorque(0);
		BackLeftWheel->SetBrakeTorque(0);
		BackRightWheel->SetBrakeTorque(0);

		//set the sideways friction to normal for all wheels
		FrontLeftWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;
		FrontRightWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;
		BackLeftWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;
		BackRightWheel->SidewaysFrictionCurve.Stiffness = NormalSidewaysStiffness;

		//set the forward friction to normal for all wheels
		FrontLeftWheel->ForwardFrictionCurve.Stiffness = NormalForwardStiffness;
		FrontRightWheel->ForwardFrictionCurve.Stiffness = NormalForwardStiffness;
		BackLeftWheel->ForwardFrictionCurve.Stiffness = NormalForwardStiffness;
		BackRightWheel->ForwardFrictionCurve.Stiffness = NormalForwardStiffness;
		
	}
	else
	{
		//makes the handbrake on
		bHandbrakeActive = true;

		//set the brake torque for all wheels
		FrontLeftWheel->SetBrakeTorque(HandbrakeTorque);
		FrontRightWheel->SetBrakeTorque(HandbrakeTorque);
		BackLeftWheel->SetBrakeTorque(HandbrakeTorque);
		BackRightWheel->SetBrakeTorque(HandbrakeTorque);

		//set the sideways friction to the handbrake one for all wheels
		FrontLeftWheel->SidewaysFrictionCurve.Stiffness = HandbrakeStiffness;
		FrontRightWheel->SidewaysFrictionCurve.Stiffness = HandbrakeStiffness;
		BackLeftWheel->SidewaysFrictionCurve.Stiffness = HandbrakeStiffness;
		BackRightWheel->SidewaysFrictionCurve.Stiffness = HandbrakeStiffness;

		//set the forward friction to the handbrake one for all wheels
		FrontLeftWheel->ForwardFrictionCurve.Stiffness = HandbrakeStiffness;
		FrontRightWheel->ForwardFrictionCurve.Stiffness = HandbrakeStiffness;
		BackLeftWheel->ForwardFrictionCurve.Stiffness = HandbrakeStiffness;
		BackRightWheel->ForwardFrictionCurve.Stiffness = HandbrakeStiffness;
		
	}

	//update all the wheels
	FrontLeftWheel->SidewaysFrictionCurve.UpdateArrays();
	FrontRightWheel->SidewaysFrictionCurve.UpdateArrays();
	BackLeftWheel->SidewaysFrictionCurve.UpdateArrays();
	BackRightWheel->SidewaysFrictionCurve.UpdateArrays();
	FrontLeftWheel->ForwardFrictionCurve.UpdateArrays();
	FrontRightWheel->ForwardFrictionCurve.UpdateArrays();
	BackLeftWheel->ForwardFrictionCurve.UpdateArrays();
	BackRightWheel->ForwardFrictionCurve.UpdateArrays();
}

//when the player first begins to shoot
void AVehicle::OnFireStart(const FInputActionValue& Value)
{
	Turret->FirePressed();
}

//while the player is firing
void AVehicle::OnFiring(const FInputActionValue& Value)
{
	Turret->FireHeld();
}

//when the player lets go of fire
void AVehicle::OnFireStop(const FInputActionValue& Value)
{
	Turret->FireReleased();
}

//when the player shifts the engine up
void AVehicle::OnEngineShiftUp(const FInputActionValue& Value)
{
	
}

//when the player shifts the engine down
void AVehicle::OnEngineShiftDown(const FInputActionValue& Value)
{
	
}

//when damage is dealt to us
void AVehicle::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	LogError("RAHHH");
}
