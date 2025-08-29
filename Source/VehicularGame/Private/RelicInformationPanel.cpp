// Fill out your copyright notice in the Description page of Project Settings.


#include "RelicInformationPanel.h"
#include "Item.h"
#include "Components/TextBlock.h"

void URelicInformationPanel::NativeConstruct()
{
	Super::NativeConstruct();
}


void URelicInformationPanel::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}

void URelicInformationPanel::DisplayItemInformation(uint8 ID)
{
	FItem& Item = UItemManager::GetItemFromIndex(ID);

	MainText->SetText(Item.Name);
	SubText->SetText(Item.Description);
}
