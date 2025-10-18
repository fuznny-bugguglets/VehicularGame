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

void UUpgradeButtonWidget::SetUpgradeID(uint8 IncomingID)
{
	UpgradeID = IncomingID;
}

void UUpgradeButtonWidget::SetMechanicWidget(UMechanicWidget* IncomingPtr)
{
	MechanicWidget = IncomingPtr;
}

void UUpgradeButtonWidget::SetText(const FText& InText)
{
	ButtonText->SetText(InText);
}

