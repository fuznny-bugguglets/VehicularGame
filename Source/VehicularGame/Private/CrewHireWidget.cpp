// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewHireWidget.h"

#include "Crew.h"
#include "InventorySubsystem.h"
#include "SuperButtonWidget.h"

void UCrewHireWidget::Setup(UCityWidget* InCity)
{
	Super::Setup(InCity);
	
	//spawns a block for each hirable crew
	for (const auto CrewID : GetInventory()->GetHirableCrew())
	{
		FCrew& Crew = UCrewManager::GetCrewFromIndex(CrewID);

		FText Main = Crew.Name;

		FString SubtextString("$");
		SubtextString.AppendInt(Crew.Cost);
		FText Subtext = FText::FromString(SubtextString);

		AddItemBlock(
			CrewID,
			Main,
			Subtext
		);
	}
}

void UCrewHireWidget::UpdateButton(uint8 CrewID)
{
	//should the button disappear?
	//i.e. is it not in the array
	if (!GetInventory()->GetHirableCrew().Contains(CrewID))
	{
		//search for the button we care about
		for (auto Button : Buttons)
		{
			//is this it?
			if (Button->GetID() == CrewID)
			{
				//delete it
				Buttons.Remove(Button);
				Button->RemoveFromParent();
				Button = nullptr;
			}
		}
	}
	
	
}
