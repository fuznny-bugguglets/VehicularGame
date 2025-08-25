#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VehicularGameState.generated.h"


class AVehicularGameMode;

UCLASS()
class VEHICULARGAME_API AVehicularGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AVehicularGameState();

	// The overall threat level of the world.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game State")
	float Difficulty;

	// Call this function to increase difficulty.
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void UpdateDifficulty(float VibrationLevel, float DeltaTime);

	//currently just dev commands
	void IncrementDifficulty();
	void DecrementDifficulty();

	//gets additional max speed
	UFUNCTION()
	float GetAdditionalMaxSpeed() const;

	//gets the max boost leve;
	UFUNCTION()
	int32 GetMaxBoostLevel() const;

	//gets the additional fire rate
	int32 GetFireRateUpgradeLevel() const;

	float GetProjectileSpreadMultiplier() const;

	float GetBumperDamage() const;


protected:
	// A multiplier to control how fast difficulty increases.
	UPROPERTY(EditDefaultsOnly, Category = "Game State")
	float DifficultyIncreaseScaleFactor;

private:
	float AdditionalMaxHealth = 0.0f;
	float AdditionalMaxSpeed = 0.0f;
	int32 MaxBoostLevel = 0;
	int32 FireRateUpgradeLevel = 0.0f;
	float ProjectileSpreadMultiplier = 0.0f;
	float AdditionalDamage = 0.0f;
	float BumperDamage = 0.0f;

	void LogError(const FString& ErrorMessage);

	UPROPERTY()
	AVehicularGameMode* VehicularGameMode = nullptr;
	
};