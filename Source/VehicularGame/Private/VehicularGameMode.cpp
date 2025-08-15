#include "VehicularGameMode.h"
#include "VehicularGameState.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "Ruin.h"
#include "MainHUD.h"

struct FSpawnPointData
{
    AActor* Point;
    double DistanceSq;

    bool operator<(const FSpawnPointData& Other) const
    {
        return DistanceSq < Other.DistanceSq;
    }
};

void AVehicularGameMode::LogError(const FString& ErrorMessage)
{
	//if we have the engine pointer, we print to the screen
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorMessage);
	}
	//otherwise, we print to the log
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorMessage);
	}
}


AVehicularGameMode::AVehicularGameMode()
{
	WaveIntervalMinutes = 4.0f;
	WaveSpawnOffsetMinutes = 1.0f;
	WaveDifficultyAmplitude = 10.0f;
	WaveDifficultyFrequency = 0.01f;
	SpawnPointPoolSize = 5;
	SpawnBatchSize = 9;
	BatchSpawnIntervalSeconds = 1.0f;
	CurrentSpawnIndex = 0;
}

void AVehicularGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("EnemySpawnPoint"), AllSpawnPoints);

	if (AllSpawnPoints.Num() > 0)
	{
		const float FirstWaveDelay = WaveIntervalMinutes * 60.0f;
		// Uses the original timer handle name.
		GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, FirstWaveDelay, false);
	}

	if(!MainHUDClass)
	{
		LogError("main hud was not set in game mode");
		return;
	}

	MainHUDInstance = CreateWidget<UMainHUD>(GetWorld(), MainHUDClass);
	if(!MainHUDInstance)
	{
		LogError("main hud failed to cast from user widget");
		return;
	}

	MainHUDInstance->AddToViewport();
}

void AVehicularGameMode::SpawnWave()
{
	// Get the game state and player pawn.
	AVehicularGameState* VehicularGameState = GetWorld()->GetGameState<AVehicularGameState>();
	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Check to see that we can spawn enemies.
	if (!VehicularGameState || !PlayerPawn || AllSpawnPoints.Num() == 0 || AvailableEnemyTypes.Num() == 0)
	{
		// Set timer for the next wave even if this one fails, to prevent spawning from stopping entirely.
		const float NextWaveDelay = WaveIntervalMinutes * 60.0f;
		GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, NextWaveDelay, false);
		return;
	}

	// --- WEIGHTED RANDOM ENEMY SPAWNING ---

	// Calculate the target difficulty for this specific wave.
	const float GameTime = GetWorld()->GetTimeSeconds();
	const float SineOffset = WaveDifficultyAmplitude * FMath::Sin(WaveDifficultyFrequency * GameTime);
	const float WaveDifficulty = FMath::Max(0.f, VehicularGameState->Difficulty + SineOffset);

	TArray<TSubclassOf<AEnemyCharacter>> EnemiesToSpawn;
	float RemainingBudget = WaveDifficulty;
	int32 Failsafe = 300; // A local failsafe to prevent excessively large waves.

	// Sort all available enemy types from cheapest to most expensive.
	TArray<FEnemyTypeData> SortedEnemyTypes = AvailableEnemyTypes;
	SortedEnemyTypes.Sort([](const FEnemyTypeData& A, const FEnemyTypeData& B) {
		return A.DifficultyCost < B.DifficultyCost;
	});

	// Loop until budget is spent or the failsafe is reached.
	while (RemainingBudget > 0 && --Failsafe > 0)
	{
		// 1. Find all enemies that are currently affordable.
		TArray<FEnemyTypeData> AffordableEnemies;
		for (const FEnemyTypeData& EnemyData : SortedEnemyTypes)
		{
			if (EnemyData.DifficultyCost > 0 && EnemyData.DifficultyCost <= RemainingBudget)
			{
				AffordableEnemies.Add(EnemyData);
			}
		}
		// If no enemies can be afforded with the remaining budget, stop.
		if (AffordableEnemies.Num() == 0)
		{
			break;
		}

		// 2. Build a "weighted pool" where cheaper enemies appear more often.
		TArray<FEnemyTypeData> WeightedSelectionPool;
		for (int32 i = 0; i < AffordableEnemies.Num(); ++i)
		{
			const int32 Weight = AffordableEnemies.Num() - i;
			for (int32 j = 0; j < Weight; ++j)
			{
				WeightedSelectionPool.Add(AffordableEnemies[i]);
			}
		}

		// 3. Pick one enemy at random from the weighted pool.
		const FEnemyTypeData& SelectedEnemy = WeightedSelectionPool[FMath::RandRange(0, WeightedSelectionPool.Num() - 1)];
		
		// 4. Spawn the enemy and remove points from the spawning budget.
		EnemiesToSpawn.Add(SelectedEnemy.EnemyClass);
		RemainingBudget -= SelectedEnemy.DifficultyCost;
	}

	// --- SPAWNING LOGIC (Find the closest points, choose random one from those and spawn there) ---
	if (EnemiesToSpawn.Num() > 0)
	{
		const FVector PlayerLocation = PlayerPawn->GetActorLocation();
		TArray<FSpawnPointData> SortedSpawnPoints;

		for (AActor* Point : AllSpawnPoints)
		{
			if (Point)
			{
				SortedSpawnPoints.Add({ Point, FVector::DistSquared(PlayerLocation, Point->GetActorLocation()) });
			}
		}
		SortedSpawnPoints.Sort();

		const int32 PoolCount = FMath::Min(SpawnPointPoolSize, SortedSpawnPoints.Num());
		if (PoolCount > 0)
		{
			const int32 RandomPoolIndex = FMath::RandRange(0, PoolCount - 1);
			AActor* FinalSpawnPoint = SortedSpawnPoints[RandomPoolIndex].Point;

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
	const float NextWaveDelay = FMath::Max(10.0f, IntervalSeconds + FMath::FRandRange(-OffsetSeconds, OffsetSeconds));
	GetWorldTimerManager().SetTimer(SpawnWaveTimerHandle, this, &AVehicularGameMode::SpawnWave, NextWaveDelay, false);
}

void AVehicularGameMode::SetHandbrake(bool InHandbrake)
{
	bIsHandbrakeOn = InHandbrake;
	DisplayRuinPrompt();
}

void AVehicularGameMode::SetRuinOverlap(ARuin* InRuin)
{
	OverlappingRuin = InRuin;
	DisplayRuinPrompt();
}

void AVehicularGameMode::DisplayRuinPrompt()
{
	if(!MainHUDInstance)
	{
		LogError("failed to access main hud instance");
		return;
	}

	if(!OverlappingRuin)
	{
		MainHUDInstance->HideRuinPrompts();
	}
	else if(OverlappingRuin->GetResourceAmount() <= 0)
	{
		MainHUDInstance->DisplayExtracted();
	}
	else if(!bIsHandbrakeOn)
	{
		MainHUDInstance->DisplayHandbrakePrompt();
	}
	else
	{
		MainHUDInstance->DisplayExtracting();
	}
}

void AVehicularGameMode::UpdateCommonLootDisplay(int32 Loot)
{
	if(!MainHUDInstance)
	{
		LogError("failed to access main hud instance");
		return;
	}

	MainHUDInstance->UpdateCommonLootDisplay(Loot);
}

void AVehicularGameMode::UpdateUncommonLootDisplay(int32 Loot)
{
	if(!MainHUDInstance)
	{
		LogError("failed to access main hud instance");
		return;
	}

	MainHUDInstance->UpdateUncommonLootDisplay(Loot);
}

void AVehicularGameMode::UpdateRareLootDisplay(int32 Loot)
{
	if(!MainHUDInstance)
	{
		LogError("failed to access main hud instance");
		return;
	}

	MainHUDInstance->UpdateRareLootDisplay(Loot);
}

