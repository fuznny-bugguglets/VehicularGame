// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeButtonWidget.h"

#include "MechanicWidget.h"
#include "Components/Button.h"
#include "UpgradeSubsystem.h"

void UUpgradeButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Button)
	{
		return;
	}

	Button->OnClicked.AddDynamic(this, &UUpgradeButtonWidget::OnButtonClicked);
}

void UUpgradeButtonWidget::OnButtonClicked()
{
	if (!MechanicWidget)
	{
		return;
	}

	MechanicWidget->DisplayUpgradeInformation(UpgradeID);
}

UUpgradeSubsystem* UUpgradeButtonWidget::GetUpgradeSubsystem()
{
	//if we already have a pointer to the subsystem, return it
	if (UpgradeSubsystem)
	{
		return UpgradeSubsystem;
	}

	//otherwise, find it
	UpgradeSubsystem = GetGameInstance()->GetSubsystem<UUpgradeSubsystem>();

	return UpgradeSubsystem;
}

void UUpgradeButtonWidget::SetupFromID(uint8 IncomingID)
{
	UpgradeID = IncomingID;
	ButtonText->SetText(UUpgradeManager::GetUpgradeFromIndex(IncomingID).Name);

	RefreshDisplay();
}

void UUpgradeButtonWidget::RefreshDisplay()
{
	//is it enabled?
	if (GetUpgradeSubsystem()->GetUpgradeEnabledStatus(UpgradeID))
	{
		//is it already yet to be unlocked??
		if (!GetUpgradeSubsystem()->GetUnlockStatus(UpgradeID))
		{
			//is it affordable?
			if (MechanicWidget->CanAffordUpgrade(UpgradeID))
			{
			
				Button->SetBackgroundColor(AffordableColour);
			}
			else
			{
				Button->SetBackgroundColor(UnaffordableColour);
			}
		}
		else
		{
			Button->SetBackgroundColor(PurchasedColour);
		}
	}
	else
	{
		Button->SetBackgroundColor(LockedColour);
	}

	
}


void UUpgradeButtonWidget::SetMechanicWidget(UMechanicWidget* IncomingPtr)
{
	MechanicWidget = IncomingPtr;
}


