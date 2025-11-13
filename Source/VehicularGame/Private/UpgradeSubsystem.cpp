// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppMemberFunctionMayBeStatic
#include "UpgradeSubsystem.h"

#include "VehicularGameInstance.h"
#include "VehicularSaveGame.h"

UUpgradeSubsystem::UUpgradeSubsystem()
{
	//set each upgrade to be 0
	for (int i = 0; i < static_cast<int>(EUpgradeType::MAX); i++)
	{
		EUpgradeType ThisUpgradeType = static_cast<EUpgradeType>(i);
		UpgradeToValueMap.Add(ThisUpgradeType, 0.0f);
	}
}

void UUpgradeSubsystem::ProcessUpgrade(const FUpgrade& Upgrade)
{
	//set the value of the upgrade
	UpgradeToValueMap[Upgrade.Type] += Upgrade.Value;

	//increment amount of upgrades purchased in that tree
	switch (Upgrade.Tree)
	{
	case EUpgradeTree::Turret:
		TurretUpgradesPurchased++;
		break;

	case EUpgradeTree::Crew:
		CrewUpgradesPurchased++;
		break;
		
	case EUpgradeTree::Car:
		TruckUpgradesPurchased++;
		break;
		
	default:
		break;
	}

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

bool UUpgradeSubsystem::GetUpgradeEnabledStatus(uint8 UpgradeID) const
{
	//switch based on tree
	switch (const FUpgrade& Upgrade = UUpgradeManager::GetUpgradeFromIndex(UpgradeID); Upgrade.Tree)
	{
	case EUpgradeTree::Turret:
		return HasEnoughUpgradesForLevel(Upgrade.Level, TurretUpgradesPurchased);

	case EUpgradeTree::Crew:
		return HasEnoughUpgradesForLevel(Upgrade.Level, CrewUpgradesPurchased);
		
	case EUpgradeTree::Car:
		return HasEnoughUpgradesForLevel(Upgrade.Level, TruckUpgradesPurchased);
		
	default: 
		return false;
	}
}

bool UUpgradeSubsystem::HasEnoughUpgradesForLevel(uint8 UpgradeLevel, uint8 UpgradesPurchased) const
{
	switch (UpgradeLevel)
	{
	case 1:
		return true;
	case 2:
		if (UpgradesPurchased >= 1) return true;
		break;
	case 3:
		if (UpgradesPurchased >= 3) return true;
		break;
	case 4:
		if (UpgradesPurchased >= 5) return true;
		break;
	case 5:
		if (UpgradesPurchased >= 8) return true;
		break;
	default:
		return false;
	}

	return false;
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
	if (const UVehicularGameInstance* VGameInstance = Cast<UVehicularGameInstance>(GetGameInstance()))
	{
		if (VGameInstance->GetSaveGameObject())
		{
			//set the city storage from the save data
			UpgradeUnlockStatusMap = VGameInstance->GetSaveGameObject()->UpgradeUnlockStatusMap;
			TurretUpgradesPurchased = VGameInstance->GetSaveGameObject()->TurretUpgradesPurchased;
			CrewUpgradesPurchased = VGameInstance->GetSaveGameObject()->CrewUpgradesPurchased;
			TruckUpgradesPurchased = VGameInstance->GetSaveGameObject()->TruckUpgradesPurchased;
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

uint8 UUpgradeSubsystem::GetTurretUpgradesPurchased() const
{
	return TurretUpgradesPurchased;
}

uint8 UUpgradeSubsystem::GetCrewUpgradesPurchased() const
{
	return CrewUpgradesPurchased;
}

uint8 UUpgradeSubsystem::GetTruckUpgradesPurchased() const
{
	return TruckUpgradesPurchased;
}


