#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AVehicle;
class AVehicularGameState;

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	IDLE UMETA(DisplayName = "Idle"),
	RUNNING UMETA(DisplayName = "Running"),
	LUNGING UMETA(DisplayName = "Lunging"),
	ATTACKING UMETA(DisplayName = "Attacking")
};

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

	virtual void Landed(const FHitResult& Hit) override;

public:
	virtual void Tick(float DeltaTime) override; // Called every frame

	void Die(); // Handles death logic

	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IsDead() const; // Checks if the enemy is dead

	UFUNCTION(BlueprintCallable)
	EEnemyState GetEnemyState() const;

private:
	
	//reference to the game state
	UPROPERTY()
	AVehicularGameState* VehicularGameState = nullptr;

	//reference to the player
	UPROPERTY()
	AVehicle* VehicleRef = nullptr;

	//how fast the enemy should move based on player speed
	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* MySpeedCurve;

	//the rate at which the enemy should slow when near the player
	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float CloseToPlayerDeceleration = 10.0f;

	//the rate at which the enemy should change their speed
	UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float SpeedChangeRatePerSec = 3.0f;

	//the rate at which the enemy should recalculate their pathfinding
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float NavUpdateDistanceScaleFactor = 2.5f;

	//the distance from the player when the enemy should lunge
	UPROPERTY(EditDefaultsOnly, Category = "Lunge", meta = (AllowPrivateAccess = "true"))
	float LungeDistance = 200.0f;

	//how fast the enemy travels while lunging
	UPROPERTY(EditDefaultsOnly, Category = "Lunge", meta = (AllowPrivateAccess = "true"))
	float LungeSpeed = 500.0f;

	//how long the enemy can lunge before it fails
	UPROPERTY(EditDefaultsOnly, Category = "Lunge", meta = (AllowPrivateAccess = "true"))
	float LungeTime = 1.0f;

	//this is the cooldown between lunges
	UPROPERTY(EditDefaultsOnly, Category = "Lunge", meta = (AllowPrivateAccess = "true"))
	float LungeCooldown = 5.0f;

	UPROPERTY()
	FVector LungeDirection = FVector::Zero();

	//how long the current lunge has been going on for
	UPROPERTY()
	float ElapsedLungeTime = 0.0f;

	//how long the lunge has been cooling down for
	UPROPERTY()
	float ElapsedLungeCooldownTime = 0.0f;

	//how far the player gets launched when attacked
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float PlayerPushForce = 250.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageToPlayer = 5.f;

	//the engine sound file
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* AttackPlayerSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundConcurrency* AttackPlayerConcurrency = nullptr;

	float TargetSpeed = 0.0f;
	float RollingAverageTargetSpeed = 0.0f;

	float TimeSinceLastNavUpdate = 0.0f;

	float PlayerMinSpeed = 30.0f;

	void UpdateSpeed(float DeltaTime);
	void RotateToGround(float DeltaTime);
	void PathfindToPoint();
	void HitByPlayer();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void LogError(const FString& ErrorMessage);

	UPROPERTY()
	EEnemyState EnemyState = EEnemyState::RUNNING;
};