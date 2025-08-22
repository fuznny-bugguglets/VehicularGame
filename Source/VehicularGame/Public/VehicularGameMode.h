#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyWaveData.h"
#include "VehicularGameMode.generated.h"

class AVehicularGameState;
class UMainHUD;
class AEnemyCharacter;
class ARuin;

UCLASS()
class VEHICULARGAME_API AVehicularGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AVehicularGameMode();
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy Spawning")
    FTimerHandle SpawnWaveTimerHandle;

    UFUNCTION(BlueprintCallable)
    ARuin* GetOverlappingRuin() const;
    
    void SetHandbrake(bool InHandbrake);
    void SetRuinOverlap(ARuin* InRuin);

    void UpdateCommonLootDisplay(int32 loot);
    void UpdateUncommonLootDisplay(int32 loot);
    void UpdateRareLootDisplay(int32 loot);

    void UpdateCurrentNoise(float Noise);
    void UpdateCurrentDifficulty(float Difficulty);

    FVector CreateEnemies(AVehicularGameState* VehicularGameState = nullptr, AActor* PlayerPawn = nullptr);

protected:
    virtual void BeginPlay() override;

    void SpawnWave();
    void SpawnNextBatch();

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
	
    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "1"))
    int32 SpawnBatchSize;

    UPROPERTY(EditDefaultsOnly, Category = "Enemy Spawning", meta = (ClampMin = "0.1"))
    float BatchSpawnIntervalSeconds;

private:
    FTimerHandle BatchSpawnTimerHandle;
    TArray<TSubclassOf<AEnemyCharacter>> WaveSpawnList;
    int32 CurrentSpawnIndex;

    UPROPERTY()
    TArray<AActor*> AllSpawnPoints;

    UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UMainHUD> MainHUDClass;

    UPROPERTY()
    UMainHUD* MainHUDInstance = nullptr;

    UPROPERTY()
    bool bIsHandbrakeOn = false;
    UPROPERTY()
    ARuin* OverlappingRuin = nullptr;

    //tells the hud which element to display
    void DisplayRuinPrompt();
    
    void LogError(const FString& ErrorMessage);
};