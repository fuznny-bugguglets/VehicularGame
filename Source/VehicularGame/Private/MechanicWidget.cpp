// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanicWidget.h"
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
		UWidget* UpgradeButtonObj = CreateWidget(GetWorld(), UpgradeButtonClass);
		
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

		default:
			break;
			
		}
		
		break;

	default:
		break;
	}

	return nullptr;
}
