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
	BumperTreeButton->OnClicked.AddDynamic(this, &UMechanicWidget::OnBumperTreeButtonClicked);
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

		//make sure it actually created
		if (!UpgradeButtonObj)
		{
			return;
		}

		//set its ID
		UpgradeButtonObj->SetUpgradeID(UUpgradeManager::GetIndexFromUpgrade(Upgrade));

		//give it a reference to us
		UpgradeButtonObj->SetMechanicWidget(this);

		//set its text
		UpgradeButtonObj->SetText(Upgrade.Name);
		
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UMechanicWidget::DisplayUpgradeInformation(uint8 UpgradeID)
{
	//set which upgrade is being viewed
	SelectedUpgradeID = UpgradeID;
	
	//get the upgrade information
	FUpgrade& Upgrade = UUpgradeManager::GetUpgradeFromIndex(UpgradeID);

	//get the inventory subsystem
	UInventorySubsystem* InventorySubsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!InventorySubsystem) return;

	//set the name to display
	NameText->SetText(Upgrade.Name);

	//build a string with the costs
	FString TotalCostString;
	TotalCostString.Append("Cost:");
	TotalCostString.Append("\n");

	//assume we can afford it
	bCanUnlockUpgrade = true;

	//loop through each cost associated with the upgrade
	for (auto [Tier, Type, Amount] : Upgrade.Cost)
	{
		//get the name of the item
		FText Name = UItemManager::GetItemFromTypeAndTier(Type, Tier).Name;

		//add to the cost string
		TotalCostString.Append(FString::FromInt(Amount));
		TotalCostString.Append("x ");
		TotalCostString.Append(Name.ToString());
		TotalCostString.Append("\n");

		//get the amount of this item the player has
		int32 ItemCount = InventorySubsystem->GetItemCountFromCityStorage(UItemManager::GetItemFromTypeAndTier(Type, Tier));

		//does the player have less than the required amount?
		if (ItemCount < Amount)
		{
			bCanUnlockUpgrade = false;
		}
	}

	//display the costs
	CostText->SetText(FText::FromString(TotalCostString));
	
	//get the upgrade subsystem
	UUpgradeSubsystem* UpgradeSubsystem = GetGameInstance()->GetSubsystem<UUpgradeSubsystem>();
	if (!UpgradeSubsystem) return;
	
	//has the player unlocked this already?
	if (UpgradeSubsystem->GetUnlockStatus(UpgradeID))
	{
		//hide the unlock button
		UnlockButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//display the unlock button
		UnlockButton->SetVisibility(ESlateVisibility::Visible);
	}

	
	//can we afford it?
	if (bCanUnlockUpgrade)
	{
		UnlockButton->SetBackgroundColor(FColor::Green);
	}
	else
	{
		UnlockButton->SetBackgroundColor(FColor::Red);
	}

	
}

void UMechanicWidget::OnTurretTreeButtonClicked()
{
	TreeSwitcher->SetActiveWidgetIndex(0);
}

void UMechanicWidget::OnBumperTreeButtonClicked()
{
	TreeSwitcher->SetActiveWidgetIndex(1);
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

	//hide the unlock button
	UnlockButton->SetVisibility(ESlateVisibility::Hidden);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "unlocked!");
}
