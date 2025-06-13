#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyWaveData.h"
#include "VehicularGameMode.generated.h"

UCLASS()
class VEHICULARGAME_API AVehicularGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AVehicularGameMode();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawning")
    FTimerHandle SpawnWaveTimerHandle;

protected:
    virtual void BeginPlay() override;

    void SpawnWave();

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning")
    TArray<FEnemyTypeData> AvailableEnemyTypes;

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "0.1"))
    float WaveIntervalMinutes;

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "0.0"))
    float WaveSpawnOffsetMinutes;

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning")
    float WaveDifficultyAmplitude;

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning")
    float WaveDifficultyFrequency;

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "1"))
    int32 SpawnPointPoolSize;

private:
    TArray<AActor*> AllSpawnPoints;
};