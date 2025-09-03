// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrewEquippedWidget.generated.h"

class UCityWidget;
class UHiredCrewButton;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UCrewEquippedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void Setup(UCityWidget* InCity);
	void UpdateSlots();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UHiredCrewButton* CrewSlot1;
	UPROPERTY(meta = (BindWidget))
	UHiredCrewButton* CrewSlot2;
	UPROPERTY(meta = (BindWidget))
	UHiredCrewButton* CrewSlot3;
	UPROPERTY(meta = (BindWidget))
	UHiredCrewButton* CrewSlot4;
	UPROPERTY(meta = (BindWidget))
	UHiredCrewButton* CrewSlot5;
	UPROPERTY(meta = (BindWidget))
	UHiredCrewButton* CrewSlot6;

	UPROPERTY()
	TArray<UHiredCrewButton*> Slots;

	UPROPERTY()
	UCityWidget* CityWidget;
};
