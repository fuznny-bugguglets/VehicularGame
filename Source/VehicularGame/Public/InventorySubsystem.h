// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Item.h"
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

	const TMap<uint8, uint32>& GetCityStorage() const;
	const TMap<uint8, uint32>& GetShop() const;

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

private:
	//maps the item index to the amount of the item
	TMap<uint8, uint32> CityStorage;
	TMap<uint8, uint32> Shop;
	
};
