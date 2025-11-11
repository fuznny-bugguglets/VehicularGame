// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Upgrades.h"
#include "UpgradeSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UUpgradeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUpgradeSubsystem();
	
	void LoadSaveData();
	
	void ProcessUpgrade(const FUpgrade& Upgrade);
	
	float GetUpgradeValue(EUpgradeType UpgradeType) const;
	bool GetUnlockStatus(uint8 UpgradeID);
	bool GetUpgradeEnabledStatus(uint8 UpgradeID) const; 
	const TMap<uint8, bool>& GetUpgradeUnlockStatusMap();
	uint8 GetTurretUpgradesPurchased() const; 
	uint8 GetCrewUpgradesPurchased() const;
	uint8 GetTruckUpgradesPurchased() const;
	

private:
	bool HasEnoughUpgradesForLevel(uint8 UpgradeLevel, uint8 UpgradesPurchased) const;

	void UnlockUpgrade(uint8 UpgradeID);
	
	//maps each upgrade type to its current value
	TMap<EUpgradeType, float> UpgradeToValueMap;

	//maps each upgrade ID to its unlocked status
	UPROPERTY()
	TMap<uint8, bool> UpgradeUnlockStatusMap;

	//the amount of upgrades purchased in a tree
	uint8 TurretUpgradesPurchased = 0;
	uint8 CrewUpgradesPurchased = 0;
	uint8 TruckUpgradesPurchased = 0;
	
};
