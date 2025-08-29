// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem.h"

UInventorySubsystem::UInventorySubsystem()
{
	CityStorage.Empty();
	AddItemToCityStorage(0, 10);
	AddItemToCityStorage(1, 10);
	AddItemToCityStorage(2, 10);
	AddItemToCityStorage(3, 10);
	AddItemToCityStorage(4, 10);
	AddItemToCityStorage(5, 10);
	AddItemToCityStorage(6, 10);
	//AddItemToCityStorage(5, 3);

}

void UInventorySubsystem::AddItemToCityStorage(uint8 ItemIndex)
{
	//does the item already exist?
	if (CityStorage.Contains(ItemIndex))
	{
		//increment the count
		CityStorage[ItemIndex]++;
	}
	else
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

void UInventorySubsystem::RemoveItemFromCityStorage(uint8 ItemIndex)
{
	//does the item exist?
	if (CityStorage.Contains(ItemIndex))
	{
		//decrement the count
		CityStorage[ItemIndex]--;

		//do we have any items left?
		if (CityStorage[ItemIndex] <= 0)
		{
			//delete it from city storage
			CityStorage.Remove(ItemIndex);
		}
	}
}

void UInventorySubsystem::RemoveItemFromCityStorage(FItem& InItem)
{
	RemoveItemFromCityStorage(UItemManager::GetIndexFromItem(InItem));
}

void UInventorySubsystem::RemoveItemFromCityStorage(uint8 ItemIndex, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		RemoveItemFromCityStorage(ItemIndex);
	}
}

void UInventorySubsystem::RemoveItemFromCityStorage(FItem& InItem, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		RemoveItemFromCityStorage(InItem);
	}
}


const TMap<uint8, uint32>& UInventorySubsystem::GetCityStorage() const
{
	return CityStorage;
}
