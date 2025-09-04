// Fill out your copyright notice in the Description page of Project Settings.


#include "CityStorageWidget.h"

#include "CityWidget.h"
#include "InventorySubsystem.h"
#include "ItemButtonWidget.h"
#include "RelicInformationPanel.h"
#include "VehicularGameInstance.h"

void UCityStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCityStorageWidget::Setup(UCityWidget* InCity)
{
	Super::Setup(InCity);
	
	UInventorySubsystem* InventorySystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();

	//spawns an item block for each unique item in the ventory 
	for (auto& Element : InventorySystem->GetCityStorage())
	{
		//city storage maps item indexes to its count
		
		//get the items name from its item index
		FText Main = UItemManager::GetItemFromIndex(Element.Key).Name;

		//get the amount we have of the item
		const int32 Count = Element.Value;
		FString SubtextString("x");
		SubtextString.Append(FString::FromInt(Count));
		FText Subtext = FText::FromString(SubtextString);
		
		AddItemBlock(
			Element.Key,
			Main,
			Subtext
			);
	}
}

void UCityStorageWidget::AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText)
{
	Super::AddItemBlock(ID, MainText, SubText);

	for (auto Button : Buttons)
	{
		if (Button)
		{
			//we are the storage
			//any items here should be sold

			UItemButtonWidget* ItemButtonWidget = Cast<UItemButtonWidget>(Button);
			if (ItemButtonWidget)
			{
				ItemButtonWidget->SetBuySellType(true);
			}
			
		}
	}
}


void UCityStorageWidget::UpdateButton(USuperButtonWidget* Button)
{
	if (!Button)
	{
		return;
	}
	
	const uint8 ItemID = Button->GetID();

	//does the item exist?
	if (GetGameInstance()->GetSubsystem<UInventorySubsystem>()->GetCityStorage().Contains(ItemID))
	{
		int32 Count = GetGameInstance()->GetSubsystem<UInventorySubsystem>()->GetCityStorage()[ItemID];
		//do we have any of the item?
		if (Count > 0)
		{
			FString SubtextString("x");
			SubtextString.Append(FString::FromInt(Count));
			FText Subtext = FText::FromString(SubtextString);
			
			Button->SetSubText(Subtext);

			return;
		}
	}

	//delete it
	Buttons.Remove(Button);
	Button->RemoveFromParent();
	Button = nullptr;

	//set information text to be nothing
	CityWidget->GetRelicInformationPanel()->DisplayNothing();
}

void UCityStorageWidget::UpdateButton(uint8 ItemID)
{
	Super::UpdateButton(ItemID);
}


void UCityStorageWidget::CreateItemBlock(uint8 ID)
{
	//get the items name from its item index
	FText Main = UItemManager::GetItemFromIndex(ID).Name;

	//sets it to 1
	const int32 Count = 1;
	FString SubtextString("x");
	SubtextString.Append(FString::FromInt(Count));
	FText Subtext = FText::FromString(SubtextString);

	AddItemBlock(
		ID,
		Main,
		Subtext
	);
}

