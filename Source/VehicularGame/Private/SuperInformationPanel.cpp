// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperInformationPanel.h"

void USuperInformationPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void USuperInformationPanel::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}
