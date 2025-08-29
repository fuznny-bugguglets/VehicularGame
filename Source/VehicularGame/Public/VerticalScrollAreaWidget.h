// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "VerticalScrollAreaWidget.generated.h"


class UCityWidget;
class UItemButtonWidget;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UVerticalScrollAreaWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	virtual void Setup(UCityWidget* InCity);
	
	UFUNCTION()
	void AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText);


protected:
	//where the item buttons will appear
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollyBox = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemButtonWidget> ItemButtonWidgetClass;

	UPROPERTY()
	UCityWidget* CityWidget = nullptr;

	
};
