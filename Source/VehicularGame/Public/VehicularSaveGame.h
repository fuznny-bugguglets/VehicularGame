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
};