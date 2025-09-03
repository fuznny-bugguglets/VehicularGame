// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem.h"

UInventorySubsystem::UInventorySubsystem()
{
	//sets hired crew inventory to default
	for (int32 i = 0; i < 6; i++)
	{
		HiredCrew[i] = 255;
	}

	//temp testing things you can delete later

	CityStorage.Empty();
	AddItemToCityStorage(0, 10);
	AddItemToCityStorage(1, 10);
	AddItemToCityStorage(2, 10);
	AddItemToCityStorage(3, 10);
	AddItemToCityStorage(4, 10);
	AddItemToCityStorage(5, 10);
	AddItemToCityStorage(6, 10);
	
	Shop.Empty();
	AddItemToShop(0);
	AddItemToShop(1);

	HirableCrew.Empty();
	AddCrewForHire(0);
	AddCrewForHire(1);
	AddCrewForHire(2);

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

void UInventorySubsystem::AddItemToShop(uint8 ItemIndex)
{
	//does the item already exist?
	if (Shop.Contains(ItemIndex))
	{
		//increment the count
		Shop[ItemIndex]++;
	}
	else
	{
		//create the item in the shop
		Shop.Emplace(ItemIndex, 1);
	}
}

void UInventorySubsystem::AddItemToShop(FItem& InItem)
{
	AddItemToShop(UItemManager::GetIndexFromItem(InItem));
}

void UInventorySubsystem::AddItemToShop(uint8 ItemIndex, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		AddItemToShop(ItemIndex);
	}
}

void UInventorySubsystem::AddItemToShop(FItem& InItem, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		AddItemToShop(InItem);
	}
}

void UInventorySubsystem::RemoveItemFromShop(uint8 ItemIndex)
{
	//does the item exist?
	if (Shop.Contains(ItemIndex))
	{
		//decrement the count
		Shop[ItemIndex]--;

		//do we have any items left?
		if (Shop[ItemIndex] <= 0)
		{
			//delete it from the shop
			Shop.Remove(ItemIndex);
		}
	}
}

void UInventorySubsystem::RemoveItemFromShop(FItem& InItem)
{
	RemoveItemFromShop(UItemManager::GetIndexFromItem(InItem));
}

void UInventorySubsystem::RemoveItemFromShop(uint8 ItemIndex, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		RemoveItemFromShop(ItemIndex);
	}
}

void UInventorySubsystem::RemoveItemFromShop(FItem& InItem, int32 Amount)
{
	for (int32 i = 0; i < Amount; i++)
	{
		RemoveItemFromShop(InItem);
	}
}


const TMap<uint8, uint32>& UInventorySubsystem::GetShop() const 
{
	return Shop;
}

const int32 UInventorySubsystem::GetMoney() const
{
	return Money;
}

void UInventorySubsystem::SetMoney(int32 NewMoney)
{
	Money = NewMoney;
}

void UInventorySubsystem::AddMoney(int32 NewMoney)
{
	Money += NewMoney;
}

void UInventorySubsystem::RemoveMoney(int32 NewMoney)
{
	Money -= NewMoney;
}

const TArray<uint8> UInventorySubsystem::GetHirableCrew() const
{
	return HirableCrew;
}

const uint8* UInventorySubsystem::GetHiredCrew() const
{
	return HiredCrew;
}

void UInventorySubsystem::AddCrewForHire(uint8 CrewIndex)
{
	HirableCrew.Emplace(CrewIndex);
}

void UInventorySubsystem::AddCrewForHire(FCrew& CrewRef)
{
	AddCrewForHire(UCrewManager::GetIndexFromCrew(CrewRef));
}

