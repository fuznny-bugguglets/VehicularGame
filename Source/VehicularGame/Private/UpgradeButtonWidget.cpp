// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeButtonWidget.h"

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
	if (!GetUpgradeSubsystem())
	{
		return;
	}

	//pass in upgrade to be processed
	GetUpgradeSubsystem()->ProcessUpgrade(UpgradeType, UpgradeValue);
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
