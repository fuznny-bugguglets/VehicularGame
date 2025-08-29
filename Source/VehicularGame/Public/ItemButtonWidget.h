// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ItemButtonWidget.generated.h"

class UVerticalScrollAreaWidget;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UItemButtonWidget : public UUserWidget
{
	GENERATED_BODY()  

public:
	void SetItemID(uint8 InID);
	void SetVerticalScrollArea(UVerticalScrollAreaWidget* InWidg);
	void SetText(const FText& InMainText, const FText& InSubText);
	void SetMainText(const FText& InText);
	void SetSubText(const FText& InText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* MainText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* SubText = nullptr;

	UPROPERTY()
	UVerticalScrollAreaWidget* VerticalScrollArea = nullptr;

	UPROPERTY()
	uint8 MyItemID = 255;
	
};
