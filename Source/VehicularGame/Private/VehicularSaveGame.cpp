#include "VehicularSaveGame.h"

UVehicularSaveGame::UVehicularSaveGame()
{
	// Set default values for when a new save game is created.
	CityStorage.Empty();
	UpgradeUnlockStatusMap.Empty();
	Money = 0;
	

	// The RuinResourceData map will be empty by default.
}