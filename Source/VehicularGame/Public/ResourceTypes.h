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
