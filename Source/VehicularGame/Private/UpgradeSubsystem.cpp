// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeSubsystem.h"

UUpgradeSubsystem::UUpgradeSubsystem()
{
	//set each upgrade to be 0
	for (int i = 0; i < (int)EUpgradeType::MAX; i++)
	{
		EUpgradeType ThisUpgradeType = (EUpgradeType)i;
		UpgradeToValueMap.Add(ThisUpgradeType, 0.0f);
	}
}


void UUpgradeSubsystem::ProcessUpgrade(EUpgradeType UpgradeType, float UpgradeValue)
{
	UpgradeToValueMap[UpgradeType] = UpgradeValue;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, "Processed Upgrade logic not written yet");
}
