// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanicWidget.h"

#include "Components/HorizontalBoxSlot.h"

void UMechanicWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UpgradeButtonClass)
	{
		return;
	}

	//create the widget
	UWidget* UpgradeButtonObj = CreateWidget(GetWorld(), UpgradeButtonClass);
	
	//add it into the tree
	TurretTreeLevel1->AddChildToHorizontalBox(UpgradeButtonObj);
	
	//try convert to a horizontal box slot
	if (UHorizontalBoxSlot* ThisHBoxSlot = Cast<UHorizontalBoxSlot>(UpgradeButtonObj->Slot))
	{
		//set size to fill
		ThisHBoxSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}
	
}
