// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemButtonWidget.h"
#include "VerticalScrollAreaWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UVerticalScrollAreaWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddItemBlock(FText& MainText, FText& SubText);

protected:
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	class UScrollBox* ScrollBox;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemButtonWidget> ItemButtonWidgetClass;

	
};
