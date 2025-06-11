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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	float Difficulty;

	// Call this function from the player vehicle's Tick to increase difficulty.
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void UpdateDifficulty(float VibrationLevel, float DeltaTime);

protected:
	// A multiplier to control how fast difficulty increases.
	UPROPERTY(EditDefaultsOnly, Category = "Game State")
	float DifficultyIncreaseScaleFactor;
};