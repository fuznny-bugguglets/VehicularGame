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

	TurretCritChance UMETA(DisplayName = "Turret Crit Chance"), //number between 0-100. 0 is never, 100 is always
	TurretCritDamage UMETA(DisplayName = "Turret Crit Damage"), //damage increase on top of base damage when a crit occurs
	TurretDamage UMETA(DisplayName = "Turret Damage"), //additional damage on top of default damage the turret will deal
	TurretAmmo UMETA(DisplayName = "Turret Ammo"), //additional ammo capacity of the turret on top of default
	TurretReloadSpeed UMETA(DisplayName = "Turret Reload Speed"), //the amount of time in seconds removed from default reload time
	TurretFireRate UMETA(DisplayName = "Turret Fire Rate"), //the amount of time in seconds removed from the default fire rate

	BumperDMG UMETA(DisplayName = "Bumper DMG"), //TODO the amount of additional damage the bumper deals on top of default
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
