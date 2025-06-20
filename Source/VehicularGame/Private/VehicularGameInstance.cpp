#include "VehicularGameInstance.h"
#include "VehicularSaveGame.h"
#include "Kismet/GameplayStatics.h"

UVehicularGameInstance::UVehicularGameInstance()
{
	SaveSlotName = TEXT("SaveSlot1");
}

void UVehicularGameInstance::Init()
{
	Super::Init();
	LoadGameData();
}

void UVehicularGameInstance::LoadGameData()
{
	// Check if the file is on disk.
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// If it exists, load it from the disk.
		SaveGameObject = Cast<UVehicularSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		// If it does not exist, create a new, default SaveGame object.
		if (SaveGameClass)
		{
			SaveGameObject = Cast<UVehicularSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		}
		else
		{
			SaveGameObject = Cast<UVehicularSaveGame>(UGameplayStatics::CreateSaveGameObject(UVehicularSaveGame::StaticClass()));
		}
	}
}

void UVehicularGameInstance::SaveGameData()
{
	if (SaveGameObject)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
	}
}

void UVehicularGameInstance::ResetSaveData()
{
	// Check if the file is on disk.
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// Delete the file from the disk.
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
	}

	// Load a fresh, default game state.
	LoadGameData();
}