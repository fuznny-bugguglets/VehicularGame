// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class ATurret;
class USpringArmComponent;
class AGSScavenger;
class UInputAction;
class UCustomWheelComponent;

UENUM()
enum class EEngineState : uint8
{
	OFF UMETA(DisplayName = "Off"),
	CRUISE UMETA(DisplayName = "Cruise"),
	BOOST1 UMETA(DisplayName = "Boost 1"),
	BOOST2 UMETA(DisplayName = "Boost 2")
};

UCLASS()
class VEHICULARGAME_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Loot Getters and Setters
	int32 GetCommonLootCount() const;
	int32 GetUncommonLootCount() const;
	int32 GetRareLootCount() const;

	void ResetAllLoot();
	void SetCommonLootCount(int32 NewValue);
	void SetUncommonLootCount(int32 NewValue);
	void SetRareLootCount(int32 NewValue);
 
private:
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Torque", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* TorqueCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Torque", meta = (AllowPrivateAccess = "true"))
	float MaxTorqueSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Torque", meta = (AllowPrivateAccess = "true"))
	float MaxMotorTorque = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Torque", meta = (AllowPrivateAccess = "true"))
	float HandbrakeTorque = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Steering", meta = (AllowPrivateAccess = "true"))
	float MaxSteerSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Steering", meta = (AllowPrivateAccess = "true"))
	float MinSteerAngle = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Steering", meta = (AllowPrivateAccess = "true"))
	float MaxSteerAngle = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Steering", meta = (AllowPrivateAccess = "true"))
	float SteerChangeSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Stiffness", meta = (AllowPrivateAccess = "true"))
	float NormalForwardStiffness = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Stiffness", meta = (AllowPrivateAccess = "true"))
	float NormalSidewaysStiffness = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Stiffness", meta = (AllowPrivateAccess = "true"))
	float DriftSidewaysStiffness = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving Feel | Stiffness", meta = (AllowPrivateAccess = "true"))
	float HandbrakeStiffness = 0.0f;
	
	//the class of the turret
	UPROPERTY(EditDefaultsOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TurretClass;

	//cooldown between hits
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float HitCooldown = 0.0f;

	//
	UPROPERTY(EditDefaultsOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	float BaseDifficultIncreasePerMinute = 0.0f;
	
	//the engine sound file
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* EngineSound;
	//how loud the engine sound can get
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMinVolume = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMaxVolume = 2.0f;
	//how fast the engine sound can get
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMinPitch = 0.6f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMaxPitch = 3.0f;
	//how fast the car needs to travel for max volume and speed
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float SpeedRequiredForMaxEngineSound = 50.0f;

	//default input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	//input for looking
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	//input for moving
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	//input for drifting
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DriftAction;
	//input for handbrake
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* HandbrakeAction;
	//input for firing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	//input for shifting the engine up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EngineShiftUpAction;
	//input for shifting the engine down
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EngineShiftDownAction;
	

	//reference to the instance of the engine sound
	UPROPERTY()
	UAudioComponent* EngineSoundInstance;

	//how much loot we have
	int32 CommonLootCount = 0;
	int32 UncommonLootCount = 0;
	int32 RareLootCount = 0;

	//reference to the game state
	UPROPERTY()
	AGSScavenger* ScavengerGameState;

	//reference to our turret
	UPROPERTY()
	ATurret* Turret;

	//how fast we are currently travelling
	float Speed = 0.0f;

	//the target angle for steering
	float TargetSteerAngle = 0.0f;

	//what our current steering angle is
	float CurrentSteerAngle = 0.0f;

	//our current health
	int32 Health = 1;
	//the max health we can have
	int32 MaxHealth = 1;

	//whether the handbreak is currently on or off
	bool bHandbrakeActive = false;

	//how long its been since we were last hit
	float TimeSinceLastHit;

	//reference to the spring arm
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	//reference to our camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	//reference to the child actor that holds the turret
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* TurretChildActorComponent;

	//wheels
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCustomWheelComponent* FrontLeftWheel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCustomWheelComponent* FrontRightWheel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCustomWheelComponent* BackLeftWheel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCustomWheelComponent* BackRightWheel;

	//current state of the engine
	EEngineState CurrentEngineState = EEngineState::OFF;
	
	//sets the speed and pitch of the engine based on speed
	UFUNCTION()
	void SetEngineSoundValues();

	//when the player looks around
	UFUNCTION()
	void OnLook(const struct FInputActionValue& Value);
	//when the player moves
	UFUNCTION()
	void OnMove(const struct FInputActionValue& Value);
	//when the player drifts
	UFUNCTION()
	void OnStartDrift(const struct FInputActionValue& Value);
	//when the player stops drifts
	UFUNCTION()
	void OnStopDrift(const struct FInputActionValue& Value);
	//when the player uses the handbreak
	UFUNCTION()
	void OnHandbreak(const struct FInputActionValue& Value);
	//when the player first begins to shoot
	UFUNCTION()
	void OnFireStart(const struct FInputActionValue& Value);
	//while the player is shooting
	UFUNCTION()
	void OnFiring(const struct FInputActionValue& Value);
	//when the player finishes shooting
	UFUNCTION()
	void OnFireStop(const struct FInputActionValue& Value);
	//when the player shifts the engine up
	UFUNCTION()
	void OnEngineShiftUp(const struct FInputActionValue& Value);
	//when the player shifts the engine down
	UFUNCTION()
	void OnEngineShiftDown(const struct FInputActionValue& Value);
	//when the player is dealt damage
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	//Sets the speed variable based on wheel velocities
	void CalculateCurrentSpeed();

	//Smoothly steer wheels to target angle
	void UpdateEngineStateOnReverse();
	void SteerWheels(float DeltaTime);

	//Updates the difficulty depending on the noise being produced
	void UpdateDifficulty(float DeltaTime);
	void UpdateExtractionProgress(float DeltaTime);
	void UpdateWorldSpeed(float DeltaTime);
	void UpdateTimeSinceLastHit(float DeltaTime);

	void LogError(const FString& ErrorMessage);
};
