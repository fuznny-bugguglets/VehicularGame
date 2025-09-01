// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
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
	//default constructor
	virtual void NativeConstruct() override;

	//called by CityWidget to init a reference to it
	UFUNCTION()
	virtual void Setup(UCityWidget* InCity);

	//creates a new button inside the scroll area (called by subclasses)
	UFUNCTION()
	virtual void AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText);

	//update the display text of each button
	UFUNCTION()
	void UpdateButtons();
	//overriden in children
	virtual void UpdateButton(UItemButtonWidget* Button);
	//converts id into button and runs other overload
	virtual void UpdateButton(uint8 ItemID);

	


protected:
	//where the item buttons will appear
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollyBox = nullptr;

	//the class of button to be created
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemButtonWidget> ItemButtonWidgetClass;

	//a reference to the city widget (used to communicate between other widgets)
	UPROPERTY()
	UCityWidget* CityWidget = nullptr;

	//all of the buttons made
	UPROPERTY()
	TArray<UItemButtonWidget*> Buttons;

	
};
