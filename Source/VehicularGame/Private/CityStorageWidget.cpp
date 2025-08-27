// Fill out your copyright notice in the Description page of Project Settings.


#include "CityStorageWidget.h"
#include "InventorySubsystem.h"

void UCityStorageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UInventorySubsystem* InventorySystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();

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
			Main,
			Subtext
			);
	}
}
