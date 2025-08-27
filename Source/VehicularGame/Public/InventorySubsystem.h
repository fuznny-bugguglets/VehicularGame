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

	//takes in an item (by index or type) and adds it to the inventory
	void AddItemToCityStorage(uint8 ItemIndex);
	void AddItemToCityStorage(FItem& InItem);
	void AddItemToCityStorage(uint8 ItemIndex, int32 Amount);
	void AddItemToCityStorage(FItem& InItem, int32 Amount);

private:
	//maps the item index to the amount of the item
	TMap<uint8, uint32> CityStorage;
	
};
