// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestBoxWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URequestBoxWidget : public UUserWidget
{
	GENERATED_BODY() 

public:
	UFUNCTION()
	void SetMainText(const FText& Text);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MainText = nullptr;
};
