#include "VehicularSaveGame.h"

UVehicularSaveGame::UVehicularSaveGame()
{
	// Set default values for when a new save game is created.
	CityStorage.Empty();
	UpgradeUnlockStatusMap.Empty();
	RequestAchievementStatus.Empty();
	RequestRedeemedStatus.Empty();
	LifetimeItemsCollected.Empty();
	LifetimeEnemyKills = 0;
	Money = 10;
	for (int32 i = 0; i < 6; i++)
	{
		HiredCrew[i] = 255;
	}
	bRelicsTutorialSeen = false;
	bCrewTutorialSeen = false;
}