// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemButtonWidget.h"
#include "Components/ScrollBox.h"
#include "VerticalScrollAreaWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UVerticalScrollAreaWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void AddItemBlock(const FText& MainText, const FText& SubText);

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollyBox = nullptr;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemButtonWidget> ItemButtonWidgetClass;

	
};
