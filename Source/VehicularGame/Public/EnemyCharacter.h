#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AVehicle;
class AVehicularGameState;

UCLASS()
class VEHICULARGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter(); // Constructor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth; // Maximum health of the enemy

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth; // Current health of the enemy

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; // Handles damage taken

protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned

public:
	virtual void Tick(float DeltaTime) override; // Called every frame

	void Die(); // Handles death logic

	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IsDead() const; // Checks if the enemy is dead

private:
	
	//reference to the game state
	UPROPERTY()
	AVehicularGameState* VehicularGameState = nullptr;

	//reference to the player
	UPROPERTY()
	AVehicle* VehicleRef = nullptr;

	//how fast the enemy should move based on player speed
	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* SpeedCurve;

	//the rate at which the enemy should slow when near the player
	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float CloseToPlayerDeceleration = 10.0f;

	//the rate at which the enemy should change their speed
	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float SpeedChangeRatePerSec = 3.0f;

	//the rate at which the enemy should recalculate their pathfinding
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float NavUpdateDistanceScaleFactor = 2.5f;

	bool bIsOverlappingWithPlayer = false;

	float TargetSpeed = 0.0f;
	float RollingAverageTargetSpeed = 0.0f;

	float TimeSinceLastNavUpdate = 0.0f;

	float PlayerMinSpeed = 30.0f;

	void UpdateSpeed(float DeltaTime);
	void RotateToGround(float DeltaTime);
	void PathfindToPoint(float DeltaTime);
	void IncrementTimeSinceLastRammed(float DeltaTime);
	void HitByPlayer();

	void LogError(const FString& ErrorMessage);
};