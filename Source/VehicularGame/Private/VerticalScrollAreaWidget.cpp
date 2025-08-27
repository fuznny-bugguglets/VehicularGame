// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalScrollAreaWidget.h"
#include "Components/ScrollBox.h"
#include "InventorySubsystem.h"

void UVerticalScrollAreaWidget::AddItemBlock(FText& MainText, FText& SubText)
{
	if (!ItemButtonWidgetClass)
	{
		return;
	}

	UItemButtonWidget* NewButton = CreateWidget<UItemButtonWidget>(GetWorld(), ItemButtonWidgetClass);
	if (!NewButton)
	{
		return;
	}

	ScrollBox->AddChild(NewButton);

	NewButton->SetText(MainText, SubText);

	
}