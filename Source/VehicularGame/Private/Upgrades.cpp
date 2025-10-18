// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrades.h"

TArray<FUpgrade> UUpgradeManager::Upgrades;

void UUpgradeManager::AddUpgrade(const FUpgrade& NewUpgrade)
{
	Upgrades.Emplace(NewUpgrade);
}

