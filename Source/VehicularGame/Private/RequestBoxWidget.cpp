// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestBoxWidget.h"

#include "Components/TextBlock.h"

void URequestBoxWidget::SetMainText(const FText& Text)
{
	MainText->SetText(Text);
}
