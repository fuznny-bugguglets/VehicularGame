// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButtonWidget.h"
#include "VerticalScrollAreaWidget.h"

void UItemButtonWidget::SetItemID(uint8 InID)
{
	MyItemID = InID;
}

void UItemButtonWidget::SetVerticalScrollArea(UVerticalScrollAreaWidget* InWidg)
{
	VerticalScrollArea = InWidg;
}


void UItemButtonWidget::SetText(const FText& InMainText, const FText& InSubText)
{
	SetMainText(InMainText);
	SetSubText(InSubText);
}

void UItemButtonWidget::SetMainText(const FText& InText)
{
	MainText->SetText(InText);
}

void UItemButtonWidget::SetSubText(const FText& InText)
{
	SubText->SetText(InText);
}


