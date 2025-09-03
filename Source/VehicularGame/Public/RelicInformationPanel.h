// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperInformationPanel.h"
#include "RelicInformationPanel.generated.h"

class UCityWidget;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URelicInformationPanel : public USuperInformationPanel
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void DisplayItemInformation(uint8 ID, bool bShouldSell);
	void DisplayNothing();
	
	
protected:
	//whether the item is being sold or bought
	UPROPERTY()
	bool bShouldSell = false;

	UPROPERTY()
	uint8 ItemID = 255;

	UFUNCTION()
	void OnButtonClick();
};
