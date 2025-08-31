// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CityWidget.generated.h"

class UShopWidget;
class UCityStorageWidget;
class URelicInformationPanel;
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

	void RemoveItem(const uint8 ID);

protected:
	//child widgets
	UPROPERTY(meta = (BindWidget))
	UCityStorageWidget* CityStorage = nullptr;
	UPROPERTY(meta = (BindWidget))
	UShopWidget* Shop = nullptr;
	UPROPERTY(meta = (BindWidget))
	URelicInformationPanel* RelicInformationPanel = nullptr;
	
};
