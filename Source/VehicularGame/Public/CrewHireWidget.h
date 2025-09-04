// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalScrollAreaWidget.h"
#include "CrewHireWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UCrewHireWidget : public UVerticalScrollAreaWidget
{
	GENERATED_BODY()

public:
	virtual void Setup(UCityWidget* InCity) override;

	virtual void UpdateButton(uint8 ItemID) override;
};
