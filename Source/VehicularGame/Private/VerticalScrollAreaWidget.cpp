// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalScrollAreaWidget.h"
#include "ItemButtonWidget.h"

void UVerticalScrollAreaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVerticalScrollAreaWidget::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}


void UVerticalScrollAreaWidget::AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText)
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

	NewButton->Setup(CityWidget);
	NewButton->SetText(MainText, SubText);
	NewButton->SetItemID(ID);

	if (ScrollyBox)
	{
		ScrollyBox->AddChild(NewButton);
	}
	
}

