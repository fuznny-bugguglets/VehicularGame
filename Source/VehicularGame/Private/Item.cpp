// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

void UItemManager::AddItem(const FItem& NewItem)
{
	Items.Emplace(NewItem);
}
