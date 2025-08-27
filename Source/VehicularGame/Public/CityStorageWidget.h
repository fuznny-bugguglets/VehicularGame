// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalScrollAreaWidget.h"
#include "CityStorageWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UCityStorageWidget : public UVerticalScrollAreaWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
};
