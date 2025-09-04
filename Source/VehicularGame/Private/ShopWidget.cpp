// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"

#include "InventorySubsystem.h"
#include "ItemButtonWidget.h"
#include "CityWidget.h"
#include "RelicInformationPanel.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UShopWidget::Setup(UCityWidget* InCity)
{
	Super::Setup(InCity);
	
	UInventorySubsystem* InventorySystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();

	//spawns an item block for each unique item in the ventory 
	for (auto& Element : InventorySystem->GetShop())
	{
		CreateItemBlock(Element.Key);
	}
}

void UShopWidget::AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText)
{
	Super::AddItemBlock(ID, MainText, SubText);

	for (auto Button : Buttons)
	{
		if (Button)
		{
			//we are the shio
			//any items here should be bought by the player
			UItemButtonWidget* ItemButtonWidget = Cast<UItemButtonWidget>(Button);
			if (ItemButtonWidget)
			{
				ItemButtonWidget->SetBuySellType(false);
			}
		}
	}
}

void UShopWidget::UpdateButton(USuperButtonWidget* Button)
{
	if (!Button)
	{
		return;
	}
	
	const uint8 ItemID = Button->GetID();

	//does the item exist?
	if (GetGameInstance()->GetSubsystem<UInventorySubsystem>()->GetShop().Contains(ItemID))
	{
		int32 Count = GetGameInstance()->GetSubsystem<UInventorySubsystem>()->GetShop()[ItemID];
		//do we have any of the item?
		if (Count > 0)
		{
			const int32 Value = UItemManager::GetItemFromIndex(ItemID).BuyPrice;
			FString SubtextString("$");
			SubtextString.Append(FString::FromInt(Value));
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

void UShopWidget::UpdateButton(uint8 ItemID)
{
	Super::UpdateButton(ItemID);
}

void UShopWidget::CreateItemBlock(uint8 ID)
{
	//shop maps item indexes to its count
	FItem& Item = UItemManager::GetItemFromIndex(ID);

	//get the items name from its item index
	FText Main = Item.Name;

	//get the price of the item
	//note: Element.Value relates to Key and Value (itemID and Count)
	//here, we are fetching the Item.Value: the price of an item
	//const int32 Value = Item.Value;
	const int32 Value = Item.BuyPrice;
	FString SubtextString("$");
	SubtextString.Append(FString::FromInt(Value));
	FText Subtext = FText::FromString(SubtextString);

	AddItemBlock(
		ID,
		Main,
		Subtext
	);
}