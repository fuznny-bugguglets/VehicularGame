// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RelicInformationPanel.generated.h"

class UCityWidget;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URelicInformationPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void Setup(UCityWidget* InCity);

	void DisplayItemInformation(uint8 ID);
	
	
protected:
	//where the information will appear
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MainText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SubText = nullptr;

	//where the button will be (either sell or buy based on children)
	UPROPERTY(meta = (BindWidget))
	UButton* InteractionButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionButtonMainText = nullptr;

	UPROPERTY()
	UCityWidget* CityWidget = nullptr;
};
