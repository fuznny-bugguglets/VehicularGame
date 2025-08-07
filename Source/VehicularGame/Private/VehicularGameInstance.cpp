#include "VehicularGameInstance.h"

#include "Vehicle.h"
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
	//checks we have an object to save to
	if (SaveGameObject)
	{
		//saves to that object
		UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
	}
	else
	{
		
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

// Called when the player successfully returns to the city.
void UVehicularGameInstance::OnRunSuccess()
{
	BankInventory();
	SaveRuinResources();
	SaveGameData();
	UGameplayStatics::OpenLevel(this, "UpgradeLevel");
}

//Transfers player resources out of their inventory and into the city bank
void UVehicularGameInstance::BankInventory()
{
	//make sure the save game object exists
	if(!SaveGameObject)
	{
		LogError(TEXT("Failed to locate save game object in VehicularGameInstance"));
		return;
	}
	
	//get the player pawn
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(PlayerPawn == nullptr)
	{
		LogError(TEXT("Failed to find a pawn in VehicularGameInstance"));
		return;
	}
		
	//cast it to the vehicle
	AVehicle* Vehicle = Cast<AVehicle>(PlayerPawn);
	if(Vehicle == nullptr)
	{
		LogError(TEXT("Failed to convert player pawn into vehicle in VehicularGameInstance"));
		return;
	}

	//increment the banked components
	SaveGameObject->BankedCommonLoot += Vehicle->GetCommonLootCount();
	SaveGameObject->BankedUncommonLoot += Vehicle->GetUncommonLootCount();
	SaveGameObject->BankedRareLoot += Vehicle->GetRareLootCount();

	//reset player inventory loot back to 0
	Vehicle->ResetAllLoot();
}

void UVehicularGameInstance::SaveRuinResources()
{
	//make sure the save game object exists
	if(!SaveGameObject)
	{
		LogError(TEXT("Failed to locate save game object in VehicularGameInstance"));
		return;
	}

	//wipes the ruin resource data
	SaveGameObject->RuinResourceData.Reset();

	//implements the data
	
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

