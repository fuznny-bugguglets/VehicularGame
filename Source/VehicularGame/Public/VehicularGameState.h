#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VehicularGameState.generated.h"

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

	//gets additional max speed
	UFUNCTION()
	float GetAdditionalMaxSpeed() const;

	//gets the max boost leve;
	UFUNCTION()
	int32 GetMaxBoostLevel() const;

protected:
	// A multiplier to control how fast difficulty increases.
	UPROPERTY(EditDefaultsOnly, Category = "Game State")
	float DifficultyIncreaseScaleFactor;

private:
	float AdditionalMaxHealth = 0.0f;
	float AdditionalMaxSpeed = 0.0f;
	int32 MaxBoostLevel = 0;
};