// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ItemButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UItemButtonWidget : public UUserWidget
{
	GENERATED_BODY()  

public:
	void SetText(FText& InMainText, FText& InSubText);
	void SetMainText(FText& InText);
	void SetSubText(FText& InText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* MainText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* SubText = nullptr;
	
};
