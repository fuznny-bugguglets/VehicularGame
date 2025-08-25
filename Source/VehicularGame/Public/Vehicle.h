// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ruin.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class AScavengerPawn;
class AVehicularGameState;
class AVehicularGameMode;
class ARuin;
class ATurret;
class USpringArmComponent;
class AGSScavenger;
class UInputAction;
class UCustomWheelComponent;
class UCameraComponent;
class UAudioComponent;
class UCurveFloat;

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

	void IncrementLootCount(EResourceType GivenType);
	void IncrementCommonLootCount();
	void IncrementUncommonLootCount();
	void IncrementRareLootCount();

	UFUNCTION(BlueprintCallable)
	float GetElapsedExtractionTime() const;
	UFUNCTION(BlueprintCallable)
	float GetTotalExtractionTime() const;

	//returns the current speed of the player
	float GetSpeed() const;

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(int32 NewHealth);

	void IncrementHealth();
	void DecrementHealth();

	bool IsHandbrakeActive() const;

	EEngineState GetEngineState() const;

	UStaticMeshComponent* GetStaticMesh() const;

	FVector GetDoorLocation() const;

	void ReturnScavenger(AScavengerPawn* Scavenger);
 
private:
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Torque", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* TorqueCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Torque", meta = (AllowPrivateAccess = "true"))
	float MaxTorqueSpeed = 90.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Torque", meta = (AllowPrivateAccess = "true"))
	float MaxMotorTorque = 10000000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Torque", meta = (AllowPrivateAccess = "true"))
	float HandbrakeTorque = 100000000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Steering", meta = (AllowPrivateAccess = "true"))
	float MaxSteerSpeed = 150.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Steering", meta = (AllowPrivateAccess = "true"))
	float MinSteerAngle = 2.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Steering", meta = (AllowPrivateAccess = "true"))
	float MaxSteerAngle = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Steering", meta = (AllowPrivateAccess = "true"))
	float SteerChangeSpeed = 60.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Stiffness", meta = (AllowPrivateAccess = "true"))
	float NormalForwardStiffness = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Stiffness", meta = (AllowPrivateAccess = "true"))
	float NormalSidewaysStiffness = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Stiffness", meta = (AllowPrivateAccess = "true"))
	float DriftSidewaysStiffness = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Stiffness", meta = (AllowPrivateAccess = "true"))
	float HandbrakeStiffness = 60.0;
	//time between holding engine shift and actually shifting 
	UPROPERTY(EditDefaultsOnly, Category = "Driving | Shifting", meta = (AllowPrivateAccess = "true"))
	float ShiftUpHoldMaxTime = 0.0f;

	//cooldown between hits
	UPROPERTY(EditDefaultsOnly, Category = "Shooting | Balancing", meta = (AllowPrivateAccess = "true"))
	float HitCooldown = 0.75f;

	UPROPERTY(EditDefaultsOnly, Category = "Scavengers", meta = (AllowPrivateAccess = "true"))
	int ScavengerCount = 3;

	//how many seconds between each scavenger exiting the truck
	UPROPERTY(EditDefaultsOnly, Category = "Scavengers", meta = (AllowPrivateAccess = "true"))
	float ScavengerExitTime = 0.5f;

	float ElapsedScavengerExitTime = 0.0f;

	//active scavengers
	UPROPERTY()
	TArray<AScavengerPawn*> ActiveScavengers;

	UPROPERTY(EditDefaultsOnly, Category = "Scavengers", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AScavengerPawn> ScavengerClass = nullptr;
	
	//how long extractions take
	UPROPERTY(EditDefaultsOnly, Category = "Depricated (Do Not Use)", meta = (AllowPrivateAccess = "true"))
	float ExtractionTimePerCommon = 8.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Depricated (Do Not Use)", meta = (AllowPrivateAccess = "true"))
	float ExtractionTimePerUncommon = 12.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Depricated (Do Not Use)", meta = (AllowPrivateAccess = "true"))
	float ExtractionTimePerRare = 16.0f;

	//the max health we can have
	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int32 MaxHealth = 1;

	//our current health
	int32 Health = 1;
	
	//
	UPROPERTY(EditDefaultsOnly, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	float BaseDifficultIncreasePerMinute = 3.0f;
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenDoorAction;
	

	//reference to the instance of the engine sound
	UPROPERTY()
	UAudioComponent* EngineSoundInstance;

	//how much loot we have
	int32 CommonLootCount = 0;
	int32 UncommonLootCount = 0;
	int32 RareLootCount = 0;

	//reference to the game state
	UPROPERTY()
	AVehicularGameState* VehicularGameState;

	//reference to the game mode
	UPROPERTY()
	AVehicularGameMode* VehicularGameMode;

	//reference to the vehicle mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* VehicleMesh;
	
	//reference to our turret
	UPROPERTY()
	ATurret* Turret;

	//how fast we are currently travelling
	float Speed = 0.0f;
	//how fast we are travelling in the world
	float WorldSpeed = 0.0f;

	//the target angle for steering
	float TargetSteerAngle = 0.0f;

	//what our current steering angle is
	float CurrentSteerAngle = 0.0f;

	
	

	//whether the handbreak is currently on or off
	bool bHandbrakeActive = false;

	//how long its been since we were last hit
	float TimeSinceLastHit = 0.0f;

	//how long we have been extracting for
	float ExtractionTime = 0.0f;

	//where we were last frame
	FVector LastTickPosition = FVector(0.0f);

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

	//wheel meshes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrontLeftWheelMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FrontRightWheelMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BackLeftWheelMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BackRightWheelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DoorLocation;

	//current state of the engine
	EEngineState CurrentEngineState = EEngineState::CRUISE;

	//how long we have held down engine shift
	float EngineUpTimestamp = 0.0f;
	float EngineDownTimestamp = 0.0f;

	//whether the engine is being held
	bool bShiftUpHeld = false;
	bool bShiftDownHeld = false;

	bool bIsDoorOpen = false;

	//the ruin we are currently overlapping with
	ARuin* OverlappingRuin = nullptr;
	
	//sets the speed and pitch of the engine based on speed
	UFUNCTION()
	void SetEngineSoundValues();

	//when the player looks around
	UFUNCTION()
	void OnLook(const struct FInputActionValue& Value);
	//when the player moves
	UFUNCTION()
	void OnMove(const struct FInputActionValue& Value);
	//when the player stops moving
	UFUNCTION()
	void OnMoveStop(const struct FInputActionValue& Value);
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
	void OnEngineShiftUpStart(const struct FInputActionValue& Value);
	UFUNCTION()
	void OnEngineShiftUpOnGoing(const struct FInputActionValue& Value);
	//when the player shifts the engine down
	UFUNCTION()
	void OnEngineShiftUpStop(const struct FInputActionValue& Value);
	UFUNCTION()
	void OnEngineShiftDown(const struct FInputActionValue& Value);
	UFUNCTION()
	void OnOpenDoor(const struct FInputActionValue& Value);
	//when the player is dealt damage
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
	//when the vehicle mesh starts to overlaps with something
	UFUNCTION()
	void OnVehicleBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,  // The component that triggered the overlap
	AActor* OtherActor,                        // The other actor involved in the overlap
	UPrimitiveComponent* OtherComp,            // The other actor's component
	int32 OtherBodyIndex,                      // Index of the body in the other component
	bool bFromSweep,                           // Whether this overlap came from movement (sweep)
	const FHitResult& SweepResult              // Hit result from sweep (if applicable)
	);
	//when the vehicle mesh stops overlapping with something
	UFUNCTION()
	void OnVehicleEndOverlap(
		UPrimitiveComponent* OverlappedComponent,  // The component on this actor that was overlapped
		AActor* OtherActor,                        // The other actor that stopped overlapping
		UPrimitiveComponent* OtherComp,            // The specific component on the other actor that stopped overlapping
		int32 OtherBodyIndex                       // Body index of the other component (used for complex physics bodies)
	);

	//Sets the speed variable based on wheel velocities
	void CalculateCurrentSpeed();

	//Smoothly steer wheels to target angle
	void UpdateEngineStateOnReverse();
	void SteerWheels(float DeltaTime);

	//Updates the difficulty depending on the noise being produced
	void UpdateDifficulty(float DeltaTime);
	void UpdateExtractionProgress(float DeltaTime);
	void ExtractOneUnit();
	void UpdateWorldSpeed(float DeltaTime);
	void UpdateTimeSinceLastHit(float DeltaTime);

	void SpawnScavengers(const float DeltaTime);


	//wrapper for CreateSound2D that doesn't auto destroy
	UAudioComponent* CreateSound2DNoDestroy(USoundBase* Sound);
	void LogError(const FString& ErrorMessage);
};
