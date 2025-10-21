#include "VehicularGameInstance.h"

#include "InventorySubsystem.h"
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

	//loads save data
	LoadGameData();

}

void UVehicularGameInstance::LoadGameData()
{
	UE_LOG(LogTemp, Display, TEXT("Loaded"));
	// Check if the file is on disk.
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		// If it exists, load it from the disk.
		SaveGameObject = Cast<UVehicularSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (!SaveGameObject)
		{
			UE_LOG(LogTemp, Display, TEXT("FAILED TO ASSIGN SAVE GAME OBJECT IN GAME INSTANCE"));
			return;
		}

		//load in the items
		GetSubsystem<UInventorySubsystem>()->LoadSaveData();
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

	//copy the city storage into the save data
	SaveGameObject->CityStorage = GetSubsystem<UInventorySubsystem>()->GetCityStorage();
	
	//saves to that object
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);

	UE_LOG(LogTemp, Display, TEXT("Saved"));
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

