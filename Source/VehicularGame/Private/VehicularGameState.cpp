#include "VehicularGameState.h"

#include "Kismet/GameplayStatics.h"
#include "VehicularGameMode.h"

void AVehicularGameState::LogError(const FString& ErrorMessage)
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

AVehicularGameState::AVehicularGameState()
{
	// Initialize game state variables.
	Difficulty = 1.0f;
	DifficultyIncreaseScaleFactor = 0.1f; // Default value, can be changed in Blueprint.
	
}

void AVehicularGameState::UpdateDifficulty(float VibrationLevel, float DeltaTime)
{
	// This formula increases difficulty based on player-generated vibrations.
	Difficulty += VibrationLevel * DeltaTime * DifficultyIncreaseScaleFactor;

	// update the display of difficulty
	if(!VehicularGameMode)
	{
		VehicularGameMode = Cast<AVehicularGameMode>(UGameplayStatics::GetGameMode(this));
		if(!VehicularGameMode)
		{
			LogError("failed to get gamemode in game state");
			return;
		}
	}

	VehicularGameMode->UpdateCurrentNoise(VibrationLevel);
	VehicularGameMode->UpdateCurrentDifficulty(Difficulty);
}

float AVehicularGameState::GetAdditionalMaxSpeed() const
{
	return AdditionalMaxSpeed;
}

int32 AVehicularGameState::GetMaxBoostLevel() const
{
	return MaxBoostLevel;
}

int32 AVehicularGameState::GetFireRateUpgradeLevel() const
{
	return FireRateUpgradeLevel;
}

float AVehicularGameState::GetProjectileSpreadMultiplier() const
{
	return ProjectileSpreadMultiplier;
}

float AVehicularGameState::GetBumperDamage() const
{
	return BumperDamage;
}



