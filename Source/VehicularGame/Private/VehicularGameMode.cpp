#include "VehicularGameMode.h"
#include "VehicularGameState.h"
#include "EnemyCharacter.h" 
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h" // For APawn

// A simple helper struct to hold a spawn point and its distance to the player.
struct FSpawnPointData
{
    AActor* Point;
    double DistanceSq; // Use squared distance to avoid expensive square root calculations.

    // Operator to allow sorting.
    bool operator<(const FSpawnPointData& Other) const
    {
        return DistanceSq < Other.DistanceSq;
    }
};

AVehicularGameMode::AVehicularGameMode()
{
    // Set default values in the constructor.
    WaveIntervalMinutes = 4.0f;
    WaveSpawnOffsetMinutes = 1.0f;
    WaveDifficultyAmplitude = 10.0f;
    WaveDifficultyFrequency = 0.01f;
    SpawnPointPoolSize = 5; // Default to considering the 5 nearest spawn points.
}

void AVehicularGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Find all actors tagged as "EnemySpawnPoint" and cache them.
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), AllSpawnPoints);

    if (AllSpawnPoints.Num() > 0)
    {
        // Start the timer for the first wave.
        const float FirstWaveDelay = WaveIntervalMinutes * 60.0f;
        GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, FirstWaveDelay, false);
    }
}

void AVehicularGameMode::SpawnWave()
{
    // Get the game state to read the current world difficulty.
    AVehicularGameState* VehicularGameState = GetWorld()->GetGameState<AVehicularGameState>();
    AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    // Basic checks to ensure we can spawn enemies.
    if (!VehicularGameState || !PlayerPawn || AllSpawnPoints.Num() == 0 || AvailableEnemyTypes.Num() == 0)
    {
        return;
    }

    // --- LOGIC TO FIND AND SELECT ENEMIES BASED ON BUDGET ---
    const float GameTime = GetWorld()->GetTimeSeconds();
    const float SineOffset = WaveDifficultyAmplitude * FMath::Sin(WaveDifficultyFrequency * GameTime);
    const float WaveDifficulty = VehicularGameState->Difficulty + SineOffset;

    TArray<TSubclassOf<AEnemyCharacter>> EnemiesToSpawn;
    float RemainingBudget = WaveDifficulty;
    int32 Failsafe = 200;

    while (RemainingBudget > 0 && --Failsafe > 0)
    {
        TArray<FEnemyTypeData> AffordableEnemies;
        for (const FEnemyTypeData& EnemyData : AvailableEnemyTypes)
        {
            if (EnemyData.DifficultyCost > 0 && EnemyData.DifficultyCost <= RemainingBudget)
            {
                AffordableEnemies.Add(EnemyData);
            }
        }
        if (AffordableEnemies.Num() > 0)
        {
            const FEnemyTypeData& SelectedType = AffordableEnemies[FMath::RandRange(0, AffordableEnemies.Num() - 1)];
            EnemiesToSpawn.Add(SelectedType.EnemyClass);
            RemainingBudget -= SelectedType.DifficultyCost;
        }
        else
        {
            break;
        }
    }

    // --- LOGIC TO FIND THE CLOSEST SPAWN POINTS ---
    if (EnemiesToSpawn.Num() > 0)
    {
        const FVector PlayerLocation = PlayerPawn->GetActorLocation();
        TArray<FSpawnPointData> SortedSpawnPoints;

        // 1. Calculate the squared distance from the player to each spawn point.
        for (AActor* Point : AllSpawnPoints)
        {
            if (Point)
            {
                const double DistSq = FVector::DistSquared(PlayerLocation, Point->GetActorLocation());
                SortedSpawnPoints.Add({ Point, DistSq });
            }
        }

        // 2. Sort the array of spawn points by distance (nearest first).
        SortedSpawnPoints.Sort();

        // 3. Determine the number of points to include in our selection pool.
        const int32 PoolCount = FMath::Min(SpawnPointPoolSize, SortedSpawnPoints.Num());

        if (PoolCount > 0)
        {
            // 4. Pick one random spawn point from the pool of closest points.
            const int32 RandomPoolIndex = FMath::RandRange(0, PoolCount - 1);
            AActor* FinalSpawnPoint = SortedSpawnPoints[RandomPoolIndex].Point;

            // 5. Spawn all enemies for this wave at that single chosen point.
            for (const TSubclassOf<AEnemyCharacter>& EnemyClass : EnemiesToSpawn)
            {
                if (EnemyClass && FinalSpawnPoint)
                {
                    GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, FinalSpawnPoint->GetActorLocation(), FinalSpawnPoint->GetActorRotation());
                }
            }
        }
    }

    // --- SET TIMER FOR THE NEXT WAVE ---
    const float IntervalSeconds = WaveIntervalMinutes * 60.0f;
    const float OffsetSeconds = WaveSpawnOffsetMinutes * 60.0f;
    const float NextWaveDelay = FMath::Max(30.0f, IntervalSeconds + FMath::FRandRange(-OffsetSeconds, OffsetSeconds));
    GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, NextWaveDelay, false);
}