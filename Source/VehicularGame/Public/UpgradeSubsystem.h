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

	void ProcessUpgrade(EUpgradeType UpgradeType, float UpgradeValue);
	void ProcessUpgrade(FUpgrade Upgrade);

	float GetUpgradeValue(EUpgradeType UpgradeType) const;

private:
	//maps each upgrade to a current value
	TMap<EUpgradeType, float> UpgradeToValueMap;
	
};
