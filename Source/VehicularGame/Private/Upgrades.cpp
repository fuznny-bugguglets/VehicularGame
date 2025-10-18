// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades.h"

TArray<FUpgrade> UUpgradeManager::Upgrades;

void UUpgradeManager::AddUpgrade(const FUpgrade& NewUpgrade)
{
	Upgrades.Emplace(NewUpgrade);
}

FUpgrade& UUpgradeManager::GetUpgradeFromIndex(const uint8 Index)
{
	return Upgrades[Index];
}


uint8 UUpgradeManager::GetIndexFromUpgrade(const FUpgrade& Upgrade)
{
	int32 OutIndex = 255;
	Upgrades.Find(Upgrade, OutIndex);

	return OutIndex;
}

const TArray<FUpgrade>& UUpgradeManager::GetUpgrades()
{
	return Upgrades;
}

void UUpgradeManager::ClearUpgrades()
{
	Upgrades.Empty();
}
