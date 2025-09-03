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

	virtual void Setup(UCityWidget* InCity) override;

	virtual void AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText) override;
	virtual void CreateItemBlock(uint8 ID) override;
	
	virtual void UpdateButton(UItemButtonWidget* Button) override;
	virtual void UpdateButton(uint8 ItemID) override;
};
