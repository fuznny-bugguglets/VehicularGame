// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Item.h"
#include "Crew.h"
#include "InventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UInventorySubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const TMap<uint8, uint32>& GetCityStorage() const;
	const TMap<uint8, uint32>& GetShop() const;

	const TArray<uint8> GetHirableCrew() const;
	const uint8* GetHiredCrew() const;

	const int32 GetMoney() const;
	void SetMoney(int32 NewMoney);
	void AddMoney(int32 NewMoney);
	void RemoveMoney(int32 NewMoney);

	//moves all items from the player inventory into city storage
	void MoveFromPlayerInventoryToCityStorage();

	//removes all items from the player inventory
	void ClearPlayerInventory();

	//adds item to current inventory
	void AddItemToPlayerInventory(uint8 ItemIndex);

	//returns the amount of items in the city storage for a given item
	int32 GetItemCountFromCityStorage(uint8 ItemIndex) const;
	int32 GetItemCountFromCityStorage(const FItem& InItem) const;

	//takes in an item (by index or type) and adds it to the inventory
	void AddItemToCityStorage(uint8 ItemIndex);
	void AddItemToCityStorage(FItem& InItem);
	void AddItemToCityStorage(uint8 ItemIndex, int32 Amount);
	void AddItemToCityStorage(FItem& InItem, int32 Amount);

	//takes in an item (by index or type) and remove it from the inventory
	void RemoveItemFromCityStorage(uint8 ItemIndex);
	void RemoveItemFromCityStorage(FItem& InItem);
	void RemoveItemFromCityStorage(uint8 ItemIndex, int32 Amount);
	void RemoveItemFromCityStorage(FItem& InItem, int32 Amount);

	//takes in an item (by index or type) and adds it to the shop
	void AddItemToShop(uint8 ItemIndex);
	void AddItemToShop(FItem& InItem);
	void AddItemToShop(uint8 ItemIndex, int32 Amount);
	void AddItemToShop(FItem& InItem, int32 Amount);

	//takes in an item (by index or type) and remove it from the shop
	void RemoveItemFromShop(uint8 ItemIndex);
	void RemoveItemFromShop(FItem& InItem);
	void RemoveItemFromShop(uint8 ItemIndex, int32 Amount);
	void RemoveItemFromShop(FItem& InItem, int32 Amount);
	
	void AddCrewForHire(uint8 CrewIndex);
	void AddCrewForHire(FCrew& CrewRef);
	void RemoveCrewForHire(uint8 CrewIndex);
	void RemoveCrewForHire(FCrew& CrewRef);

	void AddHiredCrew(uint8 CrewIndex);
	void AddHiredCrew(FCrew& CrewRef);
	void RemoveHiredCrew(uint8 CrewIndex);
	void RemoveHiredCrew(FCrew& CrewRef);

private:
	int32 Money = 0;

	//maps the item index to the amount of the item
	TMap<uint8, uint32> PlayerInventory;

	UPROPERTY()
	TMap<uint8, uint32> CityStorage;
	TMap<uint8, uint32> Shop;

	//int is crewmate index
	TArray<uint8> HirableCrew;
	uint8 HiredCrew[6]; //inventory size of 6
};
