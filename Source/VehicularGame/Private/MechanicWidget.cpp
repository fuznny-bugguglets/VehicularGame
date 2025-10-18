// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanicWidget.h"

#include "Item.h"
#include "Upgrades.h"
#include "Components/HorizontalBoxSlot.h"

void UMechanicWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UpgradeButtonClass)
	{
		return;
	}

	//loop through each upgrade and place it in the correct position
	for (FUpgrade Upgrade : UUpgradeManager::GetUpgrades())
	{
		//create the widget
		UUpgradeButtonWidget* UpgradeButtonObj = CreateWidget<UUpgradeButtonWidget>(GetWorld(), UpgradeButtonClass);

		//make sure it actually created
		if (!UpgradeButtonObj)
		{
			return;
		}

		//set its ID
		UpgradeButtonObj->SetUpgradeID(UUpgradeManager::GetIndexFromUpgrade(Upgrade));

		//give it a reference to us
		UpgradeButtonObj->SetMechanicWidget(this);
		
		//figure out which tree it belongs to
		UHorizontalBox* ThisUpgradeTree = GetUpgradeTree(Upgrade.Tree, Upgrade.Level);

		if (!ThisUpgradeTree)
		{
			return;
		}

		//add it into the tree
		ThisUpgradeTree->AddChildToHorizontalBox(UpgradeButtonObj);
	
		//try convert to a horizontal box slot
		if (UHorizontalBoxSlot* ThisHBoxSlot = Cast<UHorizontalBoxSlot>(UpgradeButtonObj->Slot))
		{
			//set size to fill
			ThisHBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

			//set padding
			ThisHBoxSlot->SetPadding(20.0f);
		}
	}
	
}

UHorizontalBox* UMechanicWidget::GetUpgradeTree(EUpgradeTree UpgradeTree, uint8 Index)
{
	//switch on the tree
	switch (UpgradeTree)
	{
	case EUpgradeTree::Turret:
		//switch on the index
		switch (Index)
		{
		case 1:
			return TurretTreeLevel1;

		case 2:
			return TurretTreeLevel2;

		case 3:
			return TurretTreeLevel3;

		case 4:
			return TurretTreeLevel4;

		case 5:
			return TurretTreeLevel5;
	
		default:
			break;
			
		}
		
		break;

	case EUpgradeTree::Bumper:
		//switch on the index
		switch (Index)
		{
		case 1:
			return BumperTreeLevel1;

		case 2:
			return BumperTreeLevel2;

		case 3:
			return BumperTreeLevel3;

		case 4:
			return BumperTreeLevel4;

		case 5:
			return BumperTreeLevel5;

		default:
			break;
		}

	case EUpgradeTree::Crew:
		//switch on the index
		switch (Index)
		{
		case 1:
				return CrewTreeLevel1;

		case 2:
				return CrewTreeLevel2;

		case 3:
				return CrewTreeLevel3;

		case 4:
				return CrewTreeLevel4;

		case 5:
				return CrewTreeLevel5;
	
		default:
			break;
			
		}

	case EUpgradeTree::Car:
		//switch on the index
		switch (Index)
		{
		case 1:
			return CarTreeLevel1;

		case 2:
			return CarTreeLevel2;

		case 3:
			return CarTreeLevel3;

		case 4:
			return CarTreeLevel4;

		case 5:
			return CarTreeLevel5;
	
		default:
			break;
			
		}

	default:
		break;
	}

	return nullptr;
}

void UMechanicWidget::DisplayUpgradeInformation(uint8 UpgradeID)
{
	//get the upgrade information
	FUpgrade& Upgrade = UUpgradeManager::GetUpgradeFromIndex(UpgradeID);

	//set the name to display
	NameText->SetText(Upgrade.Name);

	//build a string with the costs
	FString TotalCostString;
	
	TotalCostString.Append("Cost:");
	TotalCostString.Append("\n");
	
	for (auto Cost : Upgrade.Cost)
	{
		//get the name of the item
		FText Name = UItemManager::GetItemFromTypeAndTier(Cost.Type, Cost.Tier).Name;
		TotalCostString.Append(FString::FromInt(Cost.Amount));
		TotalCostString.Append("x ");
		TotalCostString.Append(Name.ToString());
		TotalCostString.Append("\n");
	}

	//display the costs
	CostText->SetText(FText::FromString(TotalCostString));
}

