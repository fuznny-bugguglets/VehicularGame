// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SuperButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API USuperButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Setup(UCityWidget* InCity);

protected:
	UPROPERTY()
	UCityWidget* CityWidget = nullptr;
};
