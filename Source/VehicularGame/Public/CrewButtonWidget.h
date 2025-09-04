// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperButtonWidget.h"
#include "CrewButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UCrewButtonWidget : public USuperButtonWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnButtonClicked();
};
