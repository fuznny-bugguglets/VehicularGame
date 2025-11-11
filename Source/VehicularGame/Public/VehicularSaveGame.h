#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VehicularSaveGame.generated.h"

UCLASS()
class VEHICULARGAME_API UVehicularSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UVehicularSaveGame();

	//city storage inventory
	UPROPERTY()
	TMap<uint8, uint32> CityStorage;

	//unlocked upgrades
	UPROPERTY()
	TMap<uint8, bool> UpgradeUnlockStatusMap;

	//achieved requests
	UPROPERTY()
	TMap<uint8, bool> RequestAchievementStatus;
	
	//redeemed requests
	UPROPERTY()
	TMap<uint8, bool> RequestRedeemedStatus;

	//
	UPROPERTY()
	TMap<uint8, uint32> LifetimeItemsCollected;

	//
	UPROPERTY()
	int32 LifetimeEnemyKills;
	
	//money
	UPROPERTY()
	int32 Money;

	UPROPERTY()
	uint8 HiredCrew[6];

	UPROPERTY(BlueprintReadWrite)
	bool bRelicsTutorialSeen;

	UPROPERTY(BlueprintReadWrite)
	bool bCrewTutorialSeen;

	UPROPERTY(BlueprintReadWrite)
	bool bMechanicTutorialSeen;
};