// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	ZERO = 0,

	TurretCritChance UMETA(DisplayName = "Turret Crit Chance"),
	TurretCritDamage UMETA(DisplayName = "Turret Crit Damage"),
	TurretDamage UMETA(DisplayName = "Turret Damage"),
	TurretAmmo UMETA(DisplayName = "Turret Ammo"),
	TurretReloadSpeed UMETA(DisplayName = "Turret Reload Speed"),
	TurretFireRate UMETA(DisplayName = "Turret Fire Rate"),

	BumperDMG UMETA(DisplayName = "Bumper DMG"),
	BumperHealth UMETA(DisplayName = "Bumper Health"),
	BumperImpactAbsorption UMETA(DisplayName = "Bumper Impact Absorption"),

	VehicleResistance UMETA(DisplayName = "Vehicle Resistance"),
	VehicleMaxHP UMETA(DisplayName = "Vehicle Max HP"),

	CrewScavengingSpeed UMETA(DisplayName = "Crew Scavenging Speed"),
	CrewMovementSpeed UMETA(DisplayName = "Crew Movement Speed"),
	CrewMaxCrew UMETA(DisplayName = "Crew Max Crew"),
	CrewHP UMETA(DisplayName = "Crew HP"),
	CrewHaulingCapacity UMETA(DisplayName = "Crew Hauling Capacity"),

	MAX
};

class VEHICULARGAME_API Upgrades
{
public:
	Upgrades();
	~Upgrades();
};
