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

	PlayerInventory.Empty();

	CityStorage.Empty();
	
	Shop.Empty();
	AddItemToShop(8);
	AddItemToShop(11);

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

void UInventorySubsystem::RemoveCrewForHire(uint8 CrewIndex)
{
	HirableCrew.Remove(CrewIndex);
}

void UInventorySubsystem::RemoveCrewForHire(FCrew& CrewRef)
{
	RemoveCrewForHire(UCrewManager::GetIndexFromCrew(CrewRef));
}

void UInventorySubsystem::AddHiredCrew(uint8 CrewIndex)
{
	for (int32 i = 0; i < 6; i++)
	{
		//if it is null
		if (HiredCrew[i] == 255)
		{
			HiredCrew[i] = CrewIndex;
			return;
		}
	}
}


void UInventorySubsystem::AddHiredCrew(FCrew& CrewRef)
{
	AddHiredCrew(UCrewManager::GetIndexFromCrew(CrewRef));
}

void UInventorySubsystem::RemoveHiredCrew(uint8 CrewIndex)
{
	for (int32 i = 0; i < 6; i++)
	{
		if (HiredCrew[i] == CrewIndex)
		{
			//set to null
			HiredCrew[i] = 255;
			return;
		}
	}
}

void UInventorySubsystem::RemoveHiredCrew(FCrew& CrewRef)
{
	RemoveHiredCrew(UCrewManager::GetIndexFromCrew(CrewRef));
}

void UInventorySubsystem::AddItemToPlayerInventory(uint8 ItemIndex)
{
	//does the item already exist?
	if (PlayerInventory.Contains(ItemIndex))
	{
		//increment the count
		PlayerInventory[ItemIndex]++;
	}
	else
	{
		//create the item in the inventory and give the player 1
		PlayerInventory.Emplace(ItemIndex, 1);
	}
}

void UInventorySubsystem::MoveFromPlayerInventoryToCityStorage()
{
	for (auto ItemPiece : PlayerInventory)
	{
		if (CityStorage.Contains(ItemPiece.Key))
		{
			CityStorage[ItemPiece.Key] += ItemPiece.Value;
		}
		else
		{
			CityStorage.Emplace(ItemPiece.Key, ItemPiece.Value);
		}
	}

	PlayerInventory.Empty();
}

int32 UInventorySubsystem::GetItemCountFromCityStorage(uint8 ItemIndex) const
{
	//is the item in the city storage?
	if (CityStorage.Contains(ItemIndex))
	{
		//return the count
		return CityStorage[ItemIndex];
	}
	else
	{
		//not in city storage, return nothing
		return 0;
	}
	
	
}

int32 UInventorySubsystem::GetItemCountFromCityStorage(const FItem& InItem) const
{
	return GetItemCountFromCityStorage(UItemManager::GetIndexFromItem(InItem));
}
