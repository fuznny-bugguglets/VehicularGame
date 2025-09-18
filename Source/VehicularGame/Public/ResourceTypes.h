// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	A UMETA(DisplayName = "A"),
	B UMETA(DisplayName = "B"),
	C UMETA(DisplayName = "C"),
	D UMETA(DisplayName = "D"),
	E UMETA(DisplayName = "E"),
	NULLRESOURCE UMETA(DisplayName = "Null Resource")
};

UENUM(BlueprintType)
enum class EResourceTier : uint8
{
	TIER1 UMETA(DisplayName = "Tier 1"),
	TIER2 UMETA(DisplayName = "Tier 2"),
	TIER3 UMETA(DisplayName = "Tier 3"),
	NULLTier UMETA(DisplayName = "Null Tier")
};