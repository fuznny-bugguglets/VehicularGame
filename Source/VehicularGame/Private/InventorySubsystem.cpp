// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem.h"

UInventorySubsystem::UInventorySubsystem()
{
	CityStorage.Empty();
	AddItemToCityStorage(0, 3);
	AddItemToCityStorage(1, 3);
	AddItemToCityStorage(3, 3);
	AddItemToCityStorage(5, 3);

}

void UInventorySubsystem::AddItemToCityStorage(uint8 ItemIndex)
{
	//does the item already exist?
	if (CityStorage.Contains(ItemIndex))
	{
		//increment the count
		CityStorage[ItemIndex]++;
	}
	{
		//create the item in the inventory and give the player 1
		CityStorage.Emplace(ItemIndex, 1);
	}
}

void UInventorySubsystem::AddItemToCityStorage(FItem& InItem)
{
	AddItemToCityStorage(UItemManager::GetIndexFromItem(InItem));
}

void UInventorySubsystem::AddItemToCityStorage(uint8 ItemIndex, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		AddItemToCityStorage(ItemIndex);
	}
}

void UInventorySubsystem::AddItemToCityStorage(FItem& InItem, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		AddItemToCityStorage(InItem);
	}
}


int32 UInventorySubsystem::GetCityStorageUniqueItemCount() const
{
	return CityStorage.Num();
}