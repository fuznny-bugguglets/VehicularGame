#include "VehicularSaveGame.h"

UVehicularSaveGame::UVehicularSaveGame()
{
	// Set default values for when a new save game is created.
	CityStorage.Empty();
	CityStorage.Emplace(6, 4);
	CityStorage.Emplace(12, 4);
	UpgradeUnlockStatusMap.Empty();
	RequestAchievementStatus.Empty();
	RequestRedeemedStatus.Empty();
	LifetimeItemsCollected.Empty();
	LifetimeEnemyKills = 0;
	Money = 0;
	for (int32 i = 0; i < 6; i++)
	{
		HiredCrew[i] = 255;
	}
	TurretUpgradesPurchased = 0;
	CrewUpgradesPurchased = 0;
	TruckUpgradesPurchased = 0;

	bCityTutorial1Complete = false;
	bCityTutorial2Complete = false;
}