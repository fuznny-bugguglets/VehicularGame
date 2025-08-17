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

	// Total resources the player has successfully banked in the city.
	UPROPERTY(BlueprintReadWrite, Category = "Save Data | Inventory")
	int32 BankedCommonLoot;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data | Inventory")
	int32 BankedUncommonLoot;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data | Inventory")
	int32 BankedRareLoot;

	// Stores the remaining resource count for each ruin in the level.
	// We use the ruin's unique actor name (FName) as the key.
	UPROPERTY(BlueprintReadWrite, Category = "Save Data | World State")
	TMap<FName, int32> RuinResourceData;
};