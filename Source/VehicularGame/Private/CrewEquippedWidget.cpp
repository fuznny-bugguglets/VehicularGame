// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewEquippedWidget.h"

#include "HiredCrewButton.h"
#include "InventorySubsystem.h"

void UCrewEquippedWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Slots.Add(CrewSlot1);
	Slots.Add(CrewSlot2);
	Slots.Add(CrewSlot3);
	Slots.Add(CrewSlot4);
	Slots.Add(CrewSlot5);
	Slots.Add(CrewSlot6);

	UpdateSlots();
}

void UCrewEquippedWidget::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
	for (auto ThisSlot : Slots)
	{
		ThisSlot->Setup(InCity);
	}
}


void UCrewEquippedWidget::UpdateSlots()
{
	UInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	
	for (int32 i = 0; i < 6; i++)
	{
		//is the equipped null
		if (Inventory->GetHiredCrew()[i] == 255)
		{
			Slots[i]->SetMainText(FText::GetEmpty());
			Slots[i]->SetID(255);
			continue;
		}
		
		Slots[i]->SetMainText(UCrewManager::GetCrewFromIndex(Inventory->GetHiredCrew()[i]).Name);
		Slots[i]->SetID(Inventory->GetHiredCrew()[i]);
	}
	
}
