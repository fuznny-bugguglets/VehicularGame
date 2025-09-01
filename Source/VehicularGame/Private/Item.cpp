// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

TArray<FItem> UItemManager::Items;

void UItemManager::AddItem(const FItem& NewItem)
{
	Items.Emplace(NewItem);
}

FItem& UItemManager::GetItemFromIndex(const uint8 Index)
{
	return Items[Index];
}


uint8 UItemManager::GetIndexFromItem(const FItem& Item)
{
	int32 OutIndex = 255;
	Items.Find(Item, OutIndex);

	return OutIndex;
}
