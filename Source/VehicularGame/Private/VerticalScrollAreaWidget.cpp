// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalScrollAreaWidget.h"
#include "InventorySubsystem.h"

void UVerticalScrollAreaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void UVerticalScrollAreaWidget::AddItemBlock(const FText& MainText, const FText& SubText)
{
	if (!ItemButtonWidgetClass)
	{
		return;
	}

	UItemButtonWidget* NewButton = CreateWidget<UItemButtonWidget>(GetWorld(), ItemButtonWidgetClass);
	if (NewButton == nullptr)
	{
		return;
	}

	NewButton->SetText(MainText, SubText);
	NewButton->SetVerticalScrollArea(this);
	NewButton->SetItemID()

	if (ScrollyBox)
	{
		ScrollyBox->AddChild(NewButton);
	}
	
}
