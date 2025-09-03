// Fill out your copyright notice in the Description page of Project Settings.


#include "CityWidget.h"

#include "CityStorageWidget.h"
#include "CrewHireWidget.h"
#include "CrewInformationPanel.h"
#include "ShopWidget.h"
#include "InventorySubsystem.h"
#include "RelicInformationPanel.h"
#include "VehicularGameInstance.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

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

	if (CrewHire)
	{
		CrewHire->Setup(this);
	}

	if (CrewInformationPanel)
	{
		CrewInformationPanel->Setup(this);
	}

	if (MoneyText)
	{
		UpdateMoney();
	}

	ExitButton->OnClicked.AddDynamic(this, &UCityWidget::OnExitButton);
	RelicsButton->OnClicked.AddDynamic(this, &UCityWidget::OnRelicsButton);
	CrewButton->OnClicked.AddDynamic(this, &UCityWidget::OnCrewButton);
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
	GetInventorySubsystem()->RemoveItemFromCityStorage(ID);
	GetInventorySubsystem()->AddMoney(UItemManager::GetItemFromIndex(ID).SellPrice);
	
	if (!CityStorage)
	{
		return;
	}

	CityStorage->UpdateButton(ID);
	UpdateMoney();
}

void UCityWidget::BuyItem(const uint8 ID)
{
	//can we afford it?
	if (GetInventorySubsystem()->GetMoney() >= UItemManager::GetItemFromIndex(ID).BuyPrice)
	{
		//changes inventory data
		GetInventorySubsystem()->RemoveItemFromShop(ID);
		GetInventorySubsystem()->AddItemToCityStorage(ID);
		GetInventorySubsystem()->RemoveMoney(UItemManager::GetItemFromIndex(ID).BuyPrice);

		//updates UI
		if (!CityStorage)
		{
			return;
		}

		if (CityStorage->DoesItemBlockExist(ID))
		{
			CityStorage->UpdateButton(ID);
		}
		else
		{
			CityStorage->CreateItemBlock(ID);
		}

		Shop->UpdateButton(ID);
		UpdateMoney();
	}

	

}

void UCityWidget::OnExitButton()
{
	UGameplayStatics::OpenLevel(this, TEXT("Main"));
}

void UCityWidget::OnRelicsButton()
{
	if (!WidgetSwitcher)
	{
		return;
	}

	//displays the relics screen
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UCityWidget::OnCrewButton()
{
	if (!WidgetSwitcher)
	{
		return;
	}

	//displays the crew screen
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

UInventorySubsystem* UCityWidget::GetInventorySubsystem()
{
	if (Inventory)
	{
		return Inventory;
	}

	Inventory = Cast<UVehicularGameInstance>(GetGameInstance())->GetSubsystem<UInventorySubsystem>();
	return Inventory;
}

void UCityWidget::UpdateMoney()
{
	const int32 Money = GetInventorySubsystem()->GetMoney();

	FString MoneyString = "$";
	MoneyString.AppendInt(Money);
	MoneyText->SetText(FText::FromString(MoneyString));
}

UCrewInformationPanel* UCityWidget::GetCrewInformationPanel() const
{
	return CrewInformationPanel;
}

void UCityWidget::HireCrewMember(const uint8 ID)
{
	//do we have enough
	if (GetInventorySubsystem()->GetMoney() >= UCrewManager::GetCrewFromIndex(ID).Cost)
	{
		//change inventory data
		GetInventorySubsystem()->RemoveCrewForHire(ID);
		GetInventorySubsystem()->AddHiredCrew(ID);
		GetInventorySubsystem()->RemoveMoney(UCrewManager::GetCrewFromIndex(ID).Cost);

		UpdateMoney();
		
		//update UI
		if (!CrewHire)
		{
			return;
		}

		CrewHire->UpdateButton(ID);
		
	}
}
