#include "VehicularGameMode.h"
#include "VehicularGameState.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void AVehicularGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Find all actors tagged as "EnemySpawnPoint" and cache them. 
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), SpawnPoints);

    if (SpawnPoints.Num() > 0)
    {
        // Start the timer for the first wave.
        const float FirstWaveDelay = WaveIntervalMinutes * 60.0f; // Convert minutes to seconds.
        GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, FirstWaveDelay, false);
    }
}

void AVehicularGameMode::SpawnWave()
{
    // Get the game state to read the current world difficulty.
    AVehicularGameState* VehicularGameState = GetWorld()->GetGameState<AVehicularGameState>();
    if (!VehicularGameState || SpawnPoints.Num() == 0 || AvailableEnemyTypes.Num() == 0)
    {
        return; // Cannot spawn enemies if required data is missing.
    }

    // Calculate the target difficulty for this specific wave. 
    const float GameTime = GetWorld()->GetTimeSeconds();
    const float SineOffset = WaveDifficultyAmplitude * FMath::Sin(WaveDifficultyFrequency * GameTime);
    const float WaveDifficulty = VehicularGameState->Difficulty + SineOffset;

    // Use the wave difficulty as a budget to select enemies. 
    TArray<TSubclassOf<AEnemyCharacter>> EnemiesToSpawn;
    float RemainingBudget = WaveDifficulty;

    while (RemainingBudget > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableEnemyTypes.Num() - 1);
        const FEnemyTypeData& SelectedType = AvailableEnemyTypes[RandomIndex];

        if (SelectedType.DifficultyCost <= RemainingBudget)
        {
            EnemiesToSpawn.Add(SelectedType.EnemyClass);
            RemainingBudget -= SelectedType.DifficultyCost;
        }
        else
        {
            // If we can't afford any more enemies, stop.
            break;
        }
    }

    // Choose a few random spawn points near the player. 
    AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn && EnemiesToSpawn.Num() > 0)
    {
        // Simple logic: just pick one random spawn point for this wave.
        // expand to find the closest points to the player.
        const int32 RandomSpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
        AActor* SpawnPoint = SpawnPoints[RandomSpawnIndex];

        // Spawn the selected enemies. 
        for (const TSubclassOf<AEnemyCharacter>& EnemyClass : EnemiesToSpawn)
        {
            if (EnemyClass)
            {
                GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
            }
        }
    }

    // Set the timer for the next wave with a random offset. 
    const float IntervalSeconds = WaveIntervalMinutes * 60.0f;
    const float OffsetSeconds = WaveSpawnOffsetMinutes * 60.0f;
    const float NextWaveDelay = FMath::Max(30.0f, IntervalSeconds + FMath::FRandRange(-OffsetSeconds, OffsetSeconds)); // Ensure delay is at least 30s.
    GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, NextWaveDelay, false);
}