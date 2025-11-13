// Fill out your copyright notice in the Description page of Project Settings.


#include "CityWidget.h"

#include "CityStorageWidget.h"
#include "CrewEquippedWidget.h"
#include "CrewHireWidget.h"
#include "CrewInformationPanel.h"
#include "ShopWidget.h"
#include "InventorySubsystem.h"
#include "RelicInformationPanel.h"
#include "RequestsWidget.h"
#include "VehicularGameInstance.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

//setup its child widgets
void UCityWidget::NativeConstruct()
{
	//move everything from the player inventory into the city storage
	GetInventorySubsystem()->MoveFromPlayerInventoryToCityStorage();

	//save the game
	UVehicularGameInstance* VGameInstance = Cast<UVehicularGameInstance>(GetGameInstance());
	if (VGameInstance)
	{
		UE_LOG(LogTemp, Display, TEXT("about to save"));
		VGameInstance->SaveGameData();
		
	}

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

	if (EquippedCrew)
	{
		EquippedCrew->Setup(this);
	}

	if (CrewInformationPanel)
	{
		CrewInformationPanel->Setup(this);
	}

	RequestsWidget->SetCityWidget(this);

	if (MoneyText)
	{
		UpdateMoney();
	}
	
	RelicsButton->OnClicked.AddDynamic(this, &UCityWidget::OnRelicsButton);
	CrewButton->OnClicked.AddDynamic(this, &UCityWidget::OnCrewButton);
	MechanicButton->OnClicked.AddDynamic(this, &UCityWidget::OnMechanicButton);
	RequestsButton->OnClicked.AddDynamic(this, &UCityWidget::OnRequestsButton);

	RelicsButton->SetBackgroundColor(FLinearColor::Green);
	CrewButton->SetBackgroundColor(FLinearColor::Gray);
	MechanicButton->SetBackgroundColor(FLinearColor::Gray);
	RequestsButton->SetBackgroundColor(FLinearColor::Gray);
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

void UCityWidget::OnRelicsButton()
{
	RelicsButton->SetBackgroundColor(FLinearColor::Gray);
	CrewButton->SetBackgroundColor(FLinearColor::Gray);
	MechanicButton->SetBackgroundColor(FLinearColor::Gray);
	RequestsButton->SetBackgroundColor(FLinearColor::Gray);

	RelicsButton->SetBackgroundColor(FLinearColor::Green);
	
	//change window to show relics widget
	SetWidgetSwitcher(0);

	//refresh the display of items
	CityStorage->UpdateButtons();
}

void UCityWidget::OnCrewButton()
{
	RelicsButton->SetBackgroundColor(FLinearColor::Gray);
	CrewButton->SetBackgroundColor(FLinearColor::Gray);
	MechanicButton->SetBackgroundColor(FLinearColor::Gray);
	RequestsButton->SetBackgroundColor(FLinearColor::Gray);

	CrewButton->SetBackgroundColor(FLinearColor::Green);
	
	SetWidgetSwitcher(1);
}

void UCityWidget::OnMechanicButton()
{
	RelicsButton->SetBackgroundColor(FLinearColor::Gray);
	CrewButton->SetBackgroundColor(FLinearColor::Gray);
	MechanicButton->SetBackgroundColor(FLinearColor::Gray);
	RequestsButton->SetBackgroundColor(FLinearColor::Gray);

	MechanicButton->SetBackgroundColor(FLinearColor::Green);
	
	SetWidgetSwitcher(2);
}

void UCityWidget::OnRequestsButton()
{
	RelicsButton->SetBackgroundColor(FLinearColor::Gray);
	CrewButton->SetBackgroundColor(FLinearColor::Gray);
	MechanicButton->SetBackgroundColor(FLinearColor::Gray);
	RequestsButton->SetBackgroundColor(FLinearColor::Gray);

	RequestsButton->SetBackgroundColor(FLinearColor::Green);
	
	SetWidgetSwitcher(3);
}


void UCityWidget::SetWidgetSwitcher(int32 index)
{
	if (!WidgetSwitcher)
	{
		return;
	}

	//displays the mechanics screen
	WidgetSwitcher->SetActiveWidgetIndex(index);
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

		if (!EquippedCrew)
		{
			return;
		}
		EquippedCrew->UpdateSlots();

		if (!CrewInformationPanel)
		{
			return;
		}

		CrewInformationPanel->HideDisplay();
		
	}
}

void UCityWidget::LayOffCrewMember(const uint8 ID)
{
	//change inventory data
	GetInventorySubsystem()->RemoveHiredCrew(ID);
	
	//update UI
	if (!EquippedCrew)
	{
		return;
	}
	EquippedCrew->UpdateSlots();

	if (!CrewInformationPanel)
	{
		return;
	}
	CrewInformationPanel->HideDisplay();
}
