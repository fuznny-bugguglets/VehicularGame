#include "VehicularSaveGame.h"

UVehicularSaveGame::UVehicularSaveGame()
{
	// Set default values for when a new save game is created.
	CityStorage.Empty();
	UpgradeUnlockStatusMap.Empty();
	RequestAchievementStatus.Empty();
	RequestRedeemedStatus.Empty();
	LifetimeEnemyKills = 0;
	Money = 0;
}