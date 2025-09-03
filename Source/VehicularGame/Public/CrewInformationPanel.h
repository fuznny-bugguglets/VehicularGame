// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuperInformationPanel.h"
#include "CrewInformationPanel.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UCrewInformationPanel : public USuperInformationPanel
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void DisplayCrewInformation(uint8 ID, bool _bIsHired);

protected:
	uint8 CrewID = 255; 

	bool bIsHired = false;

	UFUNCTION()
	void OnButtonClicked();
};
