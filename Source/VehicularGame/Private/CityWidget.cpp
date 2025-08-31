// Fill out your copyright notice in the Description page of Project Settings.


#include "CityWidget.h"

#include "CityStorageWidget.h"
#include "ShopWidget.h"
#include "InventorySubsystem.h"
#include "RelicInformationPanel.h"
#include "VehicularGameInstance.h"

//setup its child widgets
void UCityWidget::NativeConstruct()
{
	if (CityStorage)
	{
		CityStorage->Setup(this);
	}

	if (Shop)
	{
		Shop->Setup(this);
	}

	if (RelicInformationPanel)
	{
		RelicInformationPanel->Setup(this);
	}
}

//returns child widgets
UCityStorageWidget* UCityWidget::GetCityStorage() const
{
	return CityStorage;
}

UShopWidget* UCityWidget::GetShop() const
{
	return Shop;
}

URelicInformationPanel* UCityWidget::GetRelicInformationPanel() const
{
	return RelicInformationPanel;
}

void UCityWidget::SellItem(const uint8 ID)
{
	Cast<UVehicularGameInstance>(GetGameInstance())->GetSubsystem<UInventorySubsystem>()->RemoveItemFromCityStorage(ID);
	
	if (!CityStorage)
	{
		return;
	}

	CityStorage->UpdateButton(ID);
}

void UCityWidget::BuyItem(const uint8 ID)
{
	Cast<UVehicularGameInstance>(GetGameInstance())->GetSubsystem<UInventorySubsystem>()->RemoveItemFromShop(ID);
	Cast<UVehicularGameInstance>(GetGameInstance())->GetSubsystem<UInventorySubsystem>()->AddItemToCityStorage(ID);
	
	if (!CityStorage)
	{
		return;
	}

	CityStorage->UpdateButton(ID);
	Shop->UpdateButton(ID);
}

