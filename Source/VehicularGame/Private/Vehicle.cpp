// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CustomWheelComponent.h"
#include "VehicularGameState.h"
#include "Turret.h"
#include "Ruin.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

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
	VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vehicle Mesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TurretChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret"));
	FrontLeftWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Front Left Wheel"));
	FrontRightWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Front Right Wheel"));
	BackLeftWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Back Left Wheel"));
	BackRightWheel = CreateDefaultSubobject<UCustomWheelComponent>(TEXT("Back Right Wheel"));
	FrontLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front Left Wheel Mesh"));
	FrontRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Front Right Wheel Mesh"));
	BackLeftWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Left Wheel Mesh"));
	BackRightWheelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Right Wheel Mesh"));
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


	

}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	//binds damage delegate
	OnTakeAnyDamage.AddDynamic(this, &AVehicle::OnTakeDamage);
	//binds overlap delegates
	VehicleMesh->OnComponentBeginOverlap.AddDynamic(this, &AVehicle::OnVehicleBeginOverlap);
	VehicleMesh->OnComponentEndOverlap.AddDynamic(this, &AVehicle::OnVehicleEndOverlap);
	
	//get the game state
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if(GameStateBase == nullptr)
	{
		LogError(TEXT("Failed to get the game state base in vehicle"));
		return;
	}

	//get the scavenger game state
	VehicularGameState = Cast<AVehicularGameState>(GameStateBase);
	if(VehicularGameState == nullptr)
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
	
	CalculateCurrentSpeed();
	UpdateEngineStateOnReverse();
	SteerWheels(DeltaTime);
	UpdateDifficulty(DeltaTime);
	UpdateExtractionProgress(DeltaTime);
	UpdateWorldSpeed(DeltaTime);
	UpdateTimeSinceLastHit(DeltaTime);
	SetEngineSoundValues();
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
		//warn you
		if(LookAction == nullptr)
		{
			LogError("Look Action wasn't set in Vehicle");
			return;
		}
		if(MoveAction == nullptr)
		{
			LogError("Move Action wasn't set in Vehicle");
			return;
		}
		if(DriftAction == nullptr)
		{
			LogError("Drift Action wasn't set in Vehicle");
			return;
		}
		if(HandbrakeAction == nullptr)
		{
			LogError("Handbrake Action wasn't set in Vehicle");
			return;
		}
		if(FireAction == nullptr)
		{
			LogError("Fire Action wasn't set in Vehicle");
			return;
		}
		if(EngineShiftUpAction == nullptr)
		{
			LogError("Engine shift up Action wasn't set in Vehicle");
			return;
		}
		if(EngineShiftDownAction == nullptr)
		{
			LogError("Engine shift down Action wasn't set in Vehicle");
			return;
		}
		
		//bind the inputs
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVehicle::OnLook);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVehicle::OnMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AVehicle::OnMoveStop);
		EnhancedInputComponent->BindAction(DriftAction, ETriggerEvent::Started, this, &AVehicle::OnStartDrift);
		EnhancedInputComponent->BindAction(DriftAction, ETriggerEvent::Completed, this, &AVehicle::OnStopDrift);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AVehicle::OnHandbreak);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AVehicle::OnFireStart);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Ongoing, this, &AVehicle::OnFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AVehicle::OnFireStop);
		EnhancedInputComponent->BindAction(EngineShiftUpAction, ETriggerEvent::Started, this, &AVehicle::OnEngineShiftUpStart);
		EnhancedInputComponent->BindAction(EngineShiftUpAction, ETriggerEvent::Ongoing, this, &AVehicle::OnEngineShiftUpOnGoing);
		EnhancedInputComponent->BindAction(EngineShiftUpAction, ETriggerEvent::Canceled, this, &AVehicle::OnEngineShiftUpStop);
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
	float NewVolume = (((EngineMaxVolume - EngineMinVolume) / SpeedRequiredForMaxEngineSound) * (FMath::Abs(Speed))) + EngineMinVolume;
	EngineSoundInstance->SetVolumeMultiplier(NewVolume);

	//set new pitch
	float NewPitch = (((EngineMaxPitch - EngineMinPitch) / SpeedRequiredForMaxEngineSound) * (FMath::Abs(Speed))) + EngineMinPitch;
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
	float MyMaxTorque = VehicularGameState->GetAdditionalMaxSpeed() + MaxTorqueSpeed;

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
	if(TorqueCurve == nullptr)
	{
		LogError("You haven't set a Torque Curve in Vehicle");
		return;
	}
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
	float MySteerAngle = FMath::Lerp(MaxSteerAngle, MinSteerAngle, NormalizedCurrentSpeed);
	MySteerAngle *= InputVector.X;
	TargetSteerAngle = MySteerAngle;
}

//when the player stops moving
void AVehicle::OnMoveStop(const FInputActionValue& Value)
{
	//reduce the torque to nothing
	FrontLeftWheel->SetMotorTorque(0);
	FrontRightWheel->SetMotorTorque(0);
	BackLeftWheel->SetMotorTorque(0);
	BackRightWheel->SetMotorTorque(0);
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
void AVehicle::OnEngineShiftUpStart(const FInputActionValue& Value)
{
	//set time for when we have started our shifting
	EngineUpTimestamp = UGameplayStatics::GetTimeSeconds(this);

	//set that we are holding down the engine shift up
	bShiftUpHeld = true;
}

void AVehicle::OnEngineShiftUpOnGoing(const FInputActionValue& Value)
{
	//figure out how long we've held the button down for
	float ElapsedTime = UGameplayStatics::GetTimeSeconds(this) - EngineUpTimestamp;

	//is it still less than the time required?
	if(ElapsedTime <= ShiftUpHoldMaxTime)
	{
		return;
	}
	//otherwise, lets shift that engine up!

	//what state is the engine currently?
	switch (CurrentEngineState)
	{
	case EEngineState::OFF:
		//now you're cruising
		CurrentEngineState = EEngineState::CRUISE;
		break;
	case EEngineState::CRUISE:
		//can you boost?
		if(VehicularGameState->GetMaxBoostLevel() > 0)
		{
			CurrentEngineState = EEngineState::BOOST1;
		}
		break;
	case EEngineState::BOOST1:
		//can you boost boost?
		if(VehicularGameState->GetMaxBoostLevel() > 1)
		{
			CurrentEngineState = EEngineState::BOOST2;
		}
		break;
	}
	
}

void AVehicle::OnEngineShiftUpStop(const FInputActionValue& Value)
{
	//no longer shifting up
	bShiftUpHeld = false;
}

//when the player shifts the engine down
void AVehicle::OnEngineShiftDown(const FInputActionValue& Value)
{
	//what state is the engine currently?
	switch (CurrentEngineState)
	{
	case EEngineState::CRUISE:
		CurrentEngineState = EEngineState::OFF;
		break;
	case EEngineState::BOOST1:
		CurrentEngineState = EEngineState::CRUISE;
		break;
	case EEngineState::BOOST2:
		CurrentEngineState = EEngineState::BOOST1;
		break;
	}
}

//when damage is dealt to us
void AVehicle::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//can we get hit?
	if(TimeSinceLastHit <= HitCooldown)
	{
		return;
	}

	//reset the time since we were last hit
	TimeSinceLastHit = 0;

	//take damage
	Health -= Damage;

	//should we die?
	if(Health <= 0)
	{
		UGameplayStatics::OpenLevel(this, FName("Main"));
	}
}

//Sets the speed variable based on wheel velocities
void AVehicle::CalculateCurrentSpeed()
{
	//add all wheel velocity
	float MySpeed = 0.0f;
	MySpeed += FrontLeftWheel->GetWheelAngularVelocity();
	MySpeed += FrontRightWheel->GetWheelAngularVelocity();
	MySpeed += BackLeftWheel->GetWheelAngularVelocity();
	MySpeed += BackRightWheel->GetWheelAngularVelocity();

	//get the average from the 4 wheels
	MySpeed /= 4.0f;

	//get the per degree velocity
	MySpeed /= 360.0f;

	//multiply by the wheel circumference (in meters)
	MySpeed *= FrontLeftWheel->WheelRadius * 0.02f * PI;
	
	//convert from meters per second to kilometers per hour
	MySpeed *= 3.666f;

	Speed = MySpeed;
}

//Smoothly steer wheels to target angle
void AVehicle::SteerWheels(float DeltaTime)
{
	//is our target angle greater than our current angle?
	if(TargetSteerAngle > CurrentSteerAngle)
	{
		//calculate new steer angle
		CurrentSteerAngle = SteerChangeSpeed * DeltaTime + CurrentSteerAngle;

		//clamp
		if(CurrentSteerAngle > TargetSteerAngle)
		{
			CurrentSteerAngle = TargetSteerAngle;
		}
	}
	else
	{
		//calculate new steer angle
		CurrentSteerAngle = SteerChangeSpeed * DeltaTime * -1.0f + CurrentSteerAngle;

		//clamp
		if (CurrentSteerAngle < TargetSteerAngle)
		{
			CurrentSteerAngle = TargetSteerAngle;
		}
	}
	
	//set the steer angle for the front wheels
	FrontLeftWheel->SetSteerAngle(CurrentSteerAngle);
	FrontRightWheel->SetSteerAngle(CurrentSteerAngle);
}

//Updates the difficulty depending on the noise being produced
void AVehicle::UpdateDifficulty(float DeltaTime)
{
	//calculate the vibration level
	float VibrationLevel = BaseDifficultIncreasePerMinute;

	//multiply vibration based on engine state
	switch (CurrentEngineState)
	{
	case EEngineState::OFF:
		VibrationLevel *= 0.5f;
		break;
	case EEngineState::CRUISE:
		VibrationLevel *= 1.0f;
		break;
	case EEngineState::BOOST1:
		VibrationLevel *= 2.0f;
		break;
	case EEngineState::BOOST2:
		VibrationLevel *= 4.0f;
		break;
	}
	
	//divide by 60 (for minute)
	VibrationLevel /= 60.0f;

	//update the game state with our findings
	VehicularGameState->UpdateDifficulty(VibrationLevel, DeltaTime);
}

void AVehicle::UpdateEngineStateOnReverse()
{
	//are we reversing?
	if(WorldSpeed < -1.0f)
	{
		//are we boosting?
		if(CurrentEngineState == EEngineState::BOOST1 || CurrentEngineState == EEngineState::BOOST2)
		{
			//now we're cruising
			CurrentEngineState = EEngineState::CRUISE;
		}
	}
}

void AVehicle::OnVehicleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//cast the other actor to a ruin
	OverlappingRuin = Cast<ARuin>(OtherActor);

	//doesn't matter if it fails, because it returns nullptr
}

void AVehicle::OnVehicleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//are we overlapping with a ruin?
	if(OverlappingRuin == nullptr)
	{
		return;
	}

	//did we leave the ruin?
	if(OverlappingRuin == OtherActor)
	{
		//null out overlapping ruin
		OverlappingRuin = nullptr;
	}
	
	
}

void AVehicle::ExtractOneUnit()
{
	//are we overlapping with a ruin?
	if(OverlappingRuin == nullptr)
	{
		return;
	}

	//does the ruin have resources?
	if(OverlappingRuin->GetResourceAmount() <= 0)
	{
		return;
	}

	//set extraction time back to 0
	ExtractionTime = 0.0f;
	
	//take a resource
	OverlappingRuin->TakeOneResource();

	//increment count based on resource type
	switch (OverlappingRuin->GetResourceType())
	{
	case EResourceType::COMMON:
		CommonLootCount++;
		break;
	case EResourceType::UNCOMMON:
		UncommonLootCount++;
		break;
	case EResourceType::RARE:
		RareLootCount++;
		break;
	}
}

void AVehicle::UpdateExtractionProgress(float DeltaTime)
{
	//if the handbrake isnt active, do nothing
	if(!bHandbrakeActive)
	{
		ExtractionTime = 0.0f;
		return;
	}

	//if we arent overlapping with a ruin, do nothing
	if(OverlappingRuin == nullptr)
	{
		ExtractionTime = 0.0f;
		return;
	}

	//if the ruin doesnt have resources, do nothing
	if(OverlappingRuin->GetResourceAmount() <= 0)
	{
		ExtractionTime = 0.0f;
		return;
	}

	//increment extraction time
	ExtractionTime += DeltaTime;

	//can we extract the resource? if we can, extract!
	switch (OverlappingRuin->GetResourceType())
	{
	case EResourceType::COMMON:
		if(ExtractionTimePerCommon >= ExtractionTime)
		{
			ExtractOneUnit();
		}
		break;
		
	case EResourceType::UNCOMMON:
		if(ExtractionTimePerUncommon >= ExtractionTime)
		{
			ExtractOneUnit();
		}
		break;
		
	case EResourceType::RARE:
		if(ExtractionTimePerRare >= ExtractionTime)
		{
			ExtractOneUnit();
		}
		break;
	}

	
}

void AVehicle::UpdateWorldSpeed(float DeltaTime)
{
	//get the direction vector from last position to current posiiton
	FVector LastPosToCurrentVector = VehicleMesh->GetComponentLocation() - LastTickPosition;
	
	//get the magnitude of the vector
	float MySpeed = LastPosToCurrentVector.Length();

	//divide that distance by deltatime
	MySpeed /= DeltaTime;

	//convert to km/h
	MySpeed *= 0.03666;

	//figure out if we're travelling forward or backwards
	float DotResult = LastPosToCurrentVector.Dot(VehicleMesh->GetForwardVector());
	//if we're travelling backwards
	if(DotResult < 0.0f)
	{
		//negate our Speed
		MySpeed *= 1.0f;
	}

	//set the result to the new world speed
	WorldSpeed = MySpeed;

	//set the last position to where we are now
	LastTickPosition = VehicleMesh->GetComponentLocation();

	
}

void AVehicle::UpdateTimeSinceLastHit(float DeltaTime)
{
	TimeSinceLastHit += DeltaTime;
}

