// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "CityWidget.generated.h"

class UCrewEquippedWidget;
class UCrewInformationPanel;
class UTextBlock;
class UShopWidget;
class UCityStorageWidget;
class URelicInformationPanel;
class UInventorySubsystem;
class UCrewHireWidget;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UCityWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//calls setup functions on children widgets
	virtual void NativeConstruct() override;

	//returns child widgets
	UCityStorageWidget* GetCityStorage() const;
	UShopWidget* GetShop() const;
	URelicInformationPanel* GetRelicInformationPanel() const;
	UCrewInformationPanel* GetCrewInformationPanel() const;

	void SellItem(const uint8 ID);
	void BuyItem(const uint8 ID);

	void HireCrewMember(const uint8 ID);
	void LayOffCrewMember(const uint8 ID);

protected:
	//child widgets
	UPROPERTY(meta = (BindWidget))
	UCityStorageWidget* CityStorage = nullptr;

	UPROPERTY(meta = (BindWidget))
	UShopWidget* Shop = nullptr;

	UPROPERTY(meta = (BindWidget))
	URelicInformationPanel* RelicInformationPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCrewHireWidget* CrewHire = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCrewEquippedWidget* EquippedCrew = nullptr;

	UPROPERTY(meta = (BindWidget))
	UCrewInformationPanel* CrewInformationPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* RelicsButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* CrewButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyText = nullptr;


private:
	UFUNCTION()
	void OnExitButton();

	UFUNCTION()
	void OnRelicsButton();

	UFUNCTION()
	void OnCrewButton();

	UFUNCTION()
	UInventorySubsystem* GetInventorySubsystem();

	void UpdateMoney();

	UPROPERTY()
	UInventorySubsystem* Inventory = nullptr;
	
};
