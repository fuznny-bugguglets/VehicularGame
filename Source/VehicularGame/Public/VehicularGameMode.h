#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyWaveData.h" // Include our new data structure.
#include "VehicularGameMode.generated.h"

UCLASS()
class VEHICULARGAME_API AVehicularGameMode : public AGameModeBase
{
	GENERATED_BODY()

public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawning")
	FTimerHandle SpawnWaveTimerHandle; // Manages the timer for spawning waves.
	
protected:
	virtual void BeginPlay() override;

	// Called by a timer to spawn a wave of enemies.
	void SpawnWave();

	// List of all possible enemy types that can be spawned.
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning")
	TArray<FEnemyTypeData> AvailableEnemyTypes;

	// The base time between enemy waves in minutes. 
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "0.1"))
	float WaveIntervalMinutes;

	// A random time in minutes added or subtracted from the interval. 
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "0.0"))
	float WaveSpawnOffsetMinutes;

	// Used for the sine wave calculation on wave difficulty. 
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning")
	float WaveDifficultyAmplitude;

	// Used for the sine wave calculation on wave difficulty. 
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning")
	float WaveDifficultyFrequency;

private:
	TArray<AActor*> SpawnPoints; // Cached list of spawn points found in the world.
};