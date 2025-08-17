// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void HideRuinPrompts();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayHandbrakePrompt();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayExtracting();

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayExtracted();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCommonLootDisplay(int32 Loot);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUncommonLootDisplay(int32 Loot);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRareLootDisplay(int32 Loot);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentNoiseDisplay(float Noise);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentDifficultyDisplay(float Difficulty);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateResourcesExtractedBar(int32 ResourcesExtracted, int32 TotalResource);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateExtractionProgress(float CurrentExtractionTime, float TotalExtractionTime);
};
