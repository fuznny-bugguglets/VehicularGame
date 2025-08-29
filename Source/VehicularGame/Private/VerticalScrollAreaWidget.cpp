// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalScrollAreaWidget.h"
#include "ItemButtonWidget.h"

//default constructor
void UVerticalScrollAreaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

//called by CityWidget at the start
void UVerticalScrollAreaWidget::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}

//creates a new button inside the scroll (called by subclasses)
void UVerticalScrollAreaWidget::AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText)
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

	//setup button with information ID and text to display
	NewButton->Setup(CityWidget);
	NewButton->SetText(MainText, SubText);
	NewButton->SetItemID(ID);

	if (ScrollyBox)
	{
		ScrollyBox->AddChild(NewButton);
	}
	
}

