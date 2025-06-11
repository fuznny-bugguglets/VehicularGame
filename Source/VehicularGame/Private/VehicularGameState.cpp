#include "VehicularGameState.h"

AVehicularGameState::AVehicularGameState()
{
	// Initialize game state variables.
	Difficulty = 0.0f;
	DifficultyIncreaseScaleFactor = 0.1f; // Default value, can be changed in Blueprint.
}

void AVehicularGameState::UpdateDifficulty(float VibrationLevel, float DeltaTime)
{
	// This formula increases difficulty based on player-generated vibrations.
	Difficulty += VibrationLevel * DeltaTime * DifficultyIncreaseScaleFactor;
}