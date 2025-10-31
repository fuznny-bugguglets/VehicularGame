// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeSubsystem.h"

#include "VehicularGameInstance.h"
#include "VehicularSaveGame.h"

UUpgradeSubsystem::UUpgradeSubsystem()
{
	//set each upgrade to be 0
	for (int i = 0; i < (int)EUpgradeType::MAX; i++)
	{
		EUpgradeType ThisUpgradeType = (EUpgradeType)i;
		UpgradeToValueMap.Add(ThisUpgradeType, 0.0f);
	}
}

void UUpgradeSubsystem::ProcessUpgrade(const FUpgrade& Upgrade)
{
	//set the value of the upgrade
	UpgradeToValueMap[Upgrade.Type] += Upgrade.Value;

	//set the upgrade as unlocked
	UnlockUpgrade(UUpgradeManager::GetIndexFromUpgrade(Upgrade));
}


float UUpgradeSubsystem::GetUpgradeValue(EUpgradeType UpgradeType) const
{
	return UpgradeToValueMap[UpgradeType];
}

bool UUpgradeSubsystem::GetUnlockStatus(uint8 UpgradeID)
{
	//does it not exist?
	if (!UpgradeUnlockStatusMap.Contains(UpgradeID))
	{
		//if it doesn't exist, then it isn't unlocked
		UpgradeUnlockStatusMap.Emplace(UpgradeID, false);
	}
	
	return UpgradeUnlockStatusMap[UpgradeID];
}

void UUpgradeSubsystem::UnlockUpgrade(uint8 UpgradeID)
{
	//does it not exist?
	if (!UpgradeUnlockStatusMap.Contains(UpgradeID))
	{
		//create it and set as unlocked
		UpgradeUnlockStatusMap.Emplace(UpgradeID, true);
		return;
	}

	//set as unlocked
	UpgradeUnlockStatusMap[UpgradeID] = true;
}

const TMap<uint8, bool>& UUpgradeSubsystem::GetUpgradeUnlockStatusMap()
{
	return UpgradeUnlockStatusMap;
}

void UUpgradeSubsystem::LoadSaveData()
{
	//get save data
	UVehicularGameInstance* VGameInstance = Cast<UVehicularGameInstance>(GetGameInstance());
	if (VGameInstance)
	{
		if (VGameInstance->GetSaveGameObject())
		{
			//set the city storage from the save data
			UpgradeUnlockStatusMap = VGameInstance->GetSaveGameObject()->UpgradeUnlockStatusMap;
			UE_LOG(LogTemp, Display, TEXT("set upgrade unlock status map from save data"));

			//for each unlocked upgrade
			for (TPair<unsigned char, bool> Unlock : UpgradeUnlockStatusMap)
			{
				//get the upgrade
				FUpgrade& Upgrade = UUpgradeManager::GetUpgradeFromIndex(Unlock.Key);
				
				//apply the value
				UpgradeToValueMap[Upgrade.Type] += Upgrade.Value;
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("FAILED TO GET VEHICULAR SAVE OBJECT IN UPGRADE SUBSYSTEM"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("FAILED TO GET VEHICULAR GAME INSTANCE IN UPGRADE SUBSYSTEM"));
	}
}
