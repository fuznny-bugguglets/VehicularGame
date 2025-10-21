#include "VehicularGameInstance.h"

#include "VehicularSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UVehicularGameInstance::UVehicularGameInstance()
{
	SaveSlotName = TEXT("SaveSlot1");
}

void UVehicularGameInstance::Init()
{
	Super::Init();

	//loads save data
	LoadGameData();

	//inits item manager
	ItemManager = NewObject<UItemManager>(GetTransientPackage(), ItemManagerClass);
	if (ItemManager)
	{
		ItemManager->SetupItemsFromDataTable();
	}

	//inits crew manager
	CrewManager = NewObject<UCrewManager>(GetTransientPackage(), CrewManagerClass);
	if (CrewManager)
	{
		CrewManager->SetupCrewFromDataTable();
	}

	//inits upgrade manager
	UpgradeManager = NewObject<UUpgradeManager>(GetTransientPackage(), UpgradeManagerClass);
	if (UpgradeManager)
	{
		UpgradeManager->SetupItemsFromDataTable();
	}

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
		//check we gave a save game class
		if (!SaveGameClass)
		{
			//debug
			LogError("FAILED TO ACCESS SAVE DATA CLASS");

			return;
		}

		//create the save data object
		SaveGameObject = Cast<UVehicularSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));

		//save the slot
		UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);

		//load into the tutorial map
		UGameplayStatics::OpenLevel(this, "TutorialMainMenu");
	}
}

void UVehicularGameInstance::SaveGameData()
{
	//checks we have an object to save to
	if (!SaveGameObject)
	{
		LogError("NO SAVE OBJECT TO SAVE INTO");
		return;
	}

	
	
	//saves to that object
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
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

UVehicularSaveGame* UVehicularGameInstance::GetSaveGameObject() const
{
	return SaveGameObject;
}


void UVehicularGameInstance::LogError(const FString& ErrorMessage)
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

