#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VehicularGameInstance.generated.h"

class UVehicularSaveGame;

UCLASS()
class VEHICULARGAME_API UVehicularGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVehicularGameInstance();

	// Called when the player successfully returns to the city.
	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void OnRunSuccess();
	
	// Called when the Game Instance is first created.
	virtual void Init() override;

	// Saves the current SaveGameObject to a file on disk.
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void SaveGameData();

	// Loads the SaveGameObject from disk, or creates a new one if none exists.
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void LoadGameData();

	// Deletes the save file and creates a fresh save object.
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void ResetSaveData();

	//returns the save game object
	UVehicularSaveGame* GetSaveGameObject() const; 

protected:
	// The name of the file slot to save to.
	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	FString SaveSlotName;

	// A reference to our Save Game Blueprint, set in the editor. This ensures we load/create the correct blueprint with your upgrade bools.
	UPROPERTY(EditDefaultsOnly, Category = "Save Game")
	TSubclassOf<UVehicularSaveGame> SaveGameClass;

private:
	//used for debug purposes, logs errors
	UFUNCTION()
	void LogError(const FString& ErrorMessage);
	
	//Transfers player resources out of their inventory and into the city bank
	UFUNCTION()
	void BankInventory();

	//Stores resource count left in ruins (POI's)
	UFUNCTION()
	void SaveRuinResources();

	// A reference to the currently loaded SaveGame object. All other scripts will access data through this.
	UPROPERTY(VisibleAnywhere, Category = "Save Game")
	UVehicularSaveGame* SaveGameObject;

	UPROPERTY(VisibleAnywhere, Category = "Classes", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> RuinClass;
};