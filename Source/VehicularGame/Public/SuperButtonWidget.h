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

	void SetText(const FText& InMainText, const FText& InSubText);
	void SetMainText(const FText& InText);
	void SetSubText(const FText& InText);

	uint8 GetID() const;
	void SetID(uint8 InID);

protected:
	UPROPERTY()
	UCityWidget* CityWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button = nullptr;

	UPROPERTY()
	uint8 ButtonID = 255;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* MainText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* SubText = nullptr;
};
