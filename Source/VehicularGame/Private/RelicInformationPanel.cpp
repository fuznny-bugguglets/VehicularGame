// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicInformationPanel.h"

#include "CityWidget.h"
#include "InventorySubsystem.h"
#include "Item.h"
#include "VehicularGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URelicInformationPanel::NativeConstruct()
{
	Super::NativeConstruct();

	InteractionButton->OnClicked.AddDynamic(this, &URelicInformationPanel::OnButtonClick);
}


void URelicInformationPanel::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}

void URelicInformationPanel::DisplayItemInformation(uint8 ID, bool bInShouldSell)
{
	ItemID = ID;
	
	FItem& Item = UItemManager::GetItemFromIndex(ItemID);

	MainText->SetText(Item.Name);
	SubText->SetText(Item.Description);

	bShouldSell = bInShouldSell;
}

void URelicInformationPanel::DisplayNothing()
{
	MainText->SetText(FText::GetEmpty());
	SubText->SetText(FText::GetEmpty());
}


void URelicInformationPanel::OnButtonClick()
{
	//sell or buy item
	if (bShouldSell)
	{
		CityWidget->RemoveItem(ItemID);
	}
	else
	{
		//TO DO
		//CityWidget->AddItem(ItemID);
	}
}
