// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperButtonWidget.h"

void USuperButtonWidget::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}

void USuperButtonWidget::SetText(const FText& InMainText, const FText& InSubText)
{
	SetMainText(InMainText);
	SetSubText(InSubText);
}

void USuperButtonWidget::SetMainText(const FText& InText)
{
	if(!MainText)
	{
		return;
	}

	MainText->SetText(InText);
}

void USuperButtonWidget::SetSubText(const FText& InText)
{
	if(!SubText)
	{
		return;
	}

	SubText->SetText(InText);
}

uint8 USuperButtonWidget::GetID() const
{
	return ButtonID;
}

void USuperButtonWidget::SetID(uint8 InID)
{
	ButtonID = InID;
}

