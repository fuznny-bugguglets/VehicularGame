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

	DisplayNothing();
}

void URelicInformationPanel::DisplayItemInformation(uint8 ID, bool bInShouldSell)
{
	ItemID = ID;
	
	FItem& Item = UItemManager::GetItemFromIndex(ItemID);

	InteractionButton->SetVisibility(ESlateVisibility::Visible);
	
	MainText->SetText(Item.Name);
	SubText->SetText(Item.Description);

	bShouldSell = bInShouldSell;

	if (bShouldSell)
	{
		InteractionButtonMainText->SetText(FText::FromString("Sell"));
	}
	else
	{
		InteractionButtonMainText->SetText(FText::FromString("Buy"));
	}
}

void URelicInformationPanel::DisplayNothing()
{
	MainText->SetText(FText::GetEmpty());
	SubText->SetText(FText::GetEmpty());
	InteractionButtonMainText->SetText(FText::GetEmpty());
	InteractionButton->SetVisibility(ESlateVisibility::Hidden);
}


void URelicInformationPanel::OnButtonClick()
{
	//sell or buy item
	if (bShouldSell)
	{
		CityWidget->SellItem(ItemID);
	}
	else
	{
		CityWidget->BuyItem(ItemID);
	}
}
