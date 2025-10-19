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
	
	void ProcessUpgrade(const FUpgrade& Upgrade);

	float GetUpgradeValue(EUpgradeType UpgradeType) const;

	bool GetUnlockStatus(uint8 UpgradeID);
	void UnlockUpgrade(uint8 UpgradeID);

private:
	//maps each upgrade type to its current value
	TMap<EUpgradeType, float> UpgradeToValueMap;

	//maps each upgrade ID to its unlocked status
	TMap<uint8, bool> UpgradeUnlockStatusMap;
	
};
