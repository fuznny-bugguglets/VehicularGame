// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButtonWidget.h"

void UItemButtonWidget::SetText(FText& InMainText, FText& InSubText)
{
	SetMainText(InMainText);
	SetSubText(InSubText);
}

void UItemButtonWidget::SetMainText(FText& InText)
{
	MainText->SetText(InText);
}

void UItemButtonWidget::SetSubText(FText& InText)
{
	SubText->SetText(InText);
}


