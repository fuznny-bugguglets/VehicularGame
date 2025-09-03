// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crew.generated.h"

UENUM()
enum EUpgradeType
{
	TurretFireRate,
	TurretDamage,
	VehicleMoveSpeed

};

USTRUCT(BlueprintType)
struct FCrewAttribute : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Percentage; 
	
};

USTRUCT(BlueprintType)
struct FCrew : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;
	
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VEHICULARGAME_API UCrewManager : public UObject
{
	GENERATED_BODY()

};
