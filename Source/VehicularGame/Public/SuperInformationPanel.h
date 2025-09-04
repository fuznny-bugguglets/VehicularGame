// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CityWidget.h"
#include "Blueprint/UserWidget.h"
#include "SuperInformationPanel.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API USuperInformationPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Setup(UCityWidget* InCity);

protected:
	UPROPERTY()
	UCityWidget* CityWidget = nullptr;

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
};
