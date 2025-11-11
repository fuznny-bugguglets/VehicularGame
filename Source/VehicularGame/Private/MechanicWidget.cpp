// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanicWidget.h"

#include "InventorySubsystem.h"
#include "Item.h"
#include "Upgrades.h"
#include "UpgradeSubsystem.h"
#include "Components/Button.h"
#include "Components/HorizontalBoxSlot.h"

void UMechanicWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//setup bindings for the selection buttons
	TurretTreeButton->OnClicked.AddDynamic(this, &UMechanicWidget::OnTurretTreeButtonClicked);
	CrewTreeButton->OnClicked.AddDynamic(this, &UMechanicWidget::OnCrewTreeButtonClicked);
	CarTreeButton->OnClicked.AddDynamic(this, &UMechanicWidget::OnCarTreeButtonClicked);

	//setup binding for unlock button
	UnlockButton->OnClicked.AddDynamic(this, &UMechanicWidget::OnUnlockButtonClicked);

	
	//make sure we have a button class to spawn
	if (!UpgradeButtonClass)
	{
		return;
	}

	//loop through each upgrade and place it in the correct position
	for (FUpgrade Upgrade : UUpgradeManager::GetUpgrades())
	{
		//create the widget
		UUpgradeButtonWidget* UpgradeButtonObj = CreateWidget<UUpgradeButtonWidget>(GetWorld(), UpgradeButtonClass);
		UpgradeButtons.Add(UpgradeButtonObj);

		//make sure it actually created
		if (!UpgradeButtonObj)
		{
			return;
		}

		//give it a reference to us
		UpgradeButtonObj->SetMechanicWidget(this);
		
		//set its ID
		UpgradeButtonObj->SetupFromID(UUpgradeManager::GetIndexFromUpgrade(Upgrade));
		
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
			ThisHBoxSlot->SetPadding(10.0f);
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UMechanicWidget::DisplayUpgradeInformation(uint8 UpgradeID)
{
	//set which upgrade is being viewed
	SelectedUpgradeID = UpgradeID;
	
	//get the upgrade information
	FUpgrade& Upgrade = UUpgradeManager::GetUpgradeFromIndex(UpgradeID);

	//set the name to display
	NameText->SetText(Upgrade.Name);

	//string below the name
	FString TotalCostString;
	
	//is it unlocked?
	if (GetGameInstance()->GetSubsystem<UUpgradeSubsystem>()->GetUnlockStatus(UpgradeID))
	{
		TotalCostString.Append("Unlocked");
	}
	else
	{
		//build a string with the costs
		TotalCostString.Append("Cost:");
		TotalCostString.Append("\n");

		//assume we can afford it
		bCanUnlockUpgrade = CanAffordUpgrade(Upgrade);

		//get the costs of the upgrade
		for (auto [Tier, Type, Amount] : Upgrade.Cost)
		{
			//get the name of the item
			FText Name = UItemManager::GetItemFromTypeAndTier(Type, Tier).Name;

			//add to the cost string
			TotalCostString.Append(FString::FromInt(Amount));
			TotalCostString.Append("x ");
			TotalCostString.Append(Name.ToString());
			TotalCostString.Append("\n");
		}
	}

	//display the costs
	CostText->SetText(FText::FromString(TotalCostString));
	
	//get the upgrade subsystem
	UUpgradeSubsystem* UpgradeSubsystem = GetGameInstance()->GetSubsystem<UUpgradeSubsystem>();
	if (!UpgradeSubsystem) return;

	//is it enabled?
	if (UpgradeSubsystem->GetUpgradeEnabledStatus(UpgradeID))
	{
		//is it affordable?
		if (bCanUnlockUpgrade)
		{
			//is it already unlocked?
			if (UpgradeSubsystem->GetUnlockStatus(UpgradeID))
			{
				UnlockButton->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				UnlockButton->SetVisibility(ESlateVisibility::Visible);
				UnlockButton->SetBackgroundColor(FColor::Green);
			}
		}
		else
		{
			UnlockButton->SetVisibility(ESlateVisibility::Visible);
			UnlockButton->SetBackgroundColor(FColor::Red);
		}
	}
	else
	{
		UnlockButton->SetVisibility(ESlateVisibility::Hidden);
	}

	
}

void UMechanicWidget::OnTurretTreeButtonClicked()
{
	TreeSwitcher->SetActiveWidgetIndex(0);
}

void UMechanicWidget::OnCrewTreeButtonClicked()
{
	TreeSwitcher->SetActiveWidgetIndex(2);
}

void UMechanicWidget::OnCarTreeButtonClicked()
{
	TreeSwitcher->SetActiveWidgetIndex(3);
}

void UMechanicWidget::OnUnlockButtonClicked()
{
	//can we afford it?
	if (!bCanUnlockUpgrade) return;
	
	//get the upgrade information
	FUpgrade& Upgrade = UUpgradeManager::GetUpgradeFromIndex(SelectedUpgradeID);

	//get the inventory subsystem
	UInventorySubsystem* InventorySubsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!InventorySubsystem) return;

	//loop through each cost associated with the upgrade
	for (auto [Tier, Type, Amount] : Upgrade.Cost)
	{
		//get the item
		FItem& ThisItem = UItemManager::GetItemFromTypeAndTier(Type, Tier);

		//remove the item(s) from the player city storage
		InventorySubsystem->RemoveItemFromCityStorage(ThisItem, Amount);
	}

	//get the upgrade subsystem
	UUpgradeSubsystem* UpgradeSubsystem = GetGameInstance()->GetSubsystem<UUpgradeSubsystem>();
	if (!UpgradeSubsystem) return;

	//apply the upgrade
	UpgradeSubsystem->ProcessUpgrade(Upgrade);

	//refresh display
	DisplayUpgradeInformation(SelectedUpgradeID);

	//refresh upgrade buttons
	for (auto UpgradeButton : UpgradeButtons)
	{
		UpgradeButton->RefreshDisplay();
	}
}

void UMechanicWidget::HideInformationPanel()
{
	//hide the purchase button
	UnlockButton->SetVisibility(ESlateVisibility::Hidden);

	//hide the text
	NameText->SetText(FText::GetEmpty());
	CostText->SetText(FText::GetEmpty());
}

bool UMechanicWidget::CanAffordUpgrade(const uint8 Index) const
{
	return CanAffordUpgrade(UUpgradeManager::GetUpgradeFromIndex(Index));
}

bool UMechanicWidget::CanAffordUpgrade(const FUpgrade& Upgrade) const
{
	if (!GetGameInstance()) return false;
	
	//grab the inventory subsystem
	UInventorySubsystem* InventorySubsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!InventorySubsystem) return false;
	
	//loop through each cost associated with the upgrade
	for (auto [Tier, Type, Amount] : Upgrade.Cost)
	{
		//get the amount of this item the player has
		int32 ItemCount = InventorySubsystem->GetItemCountFromCityStorage(UItemManager::GetItemFromTypeAndTier(Type, Tier));

		//does the player have less than the required amount?
		if (ItemCount < Amount)
		{
			return false;
		}
	}

	return true;
}

