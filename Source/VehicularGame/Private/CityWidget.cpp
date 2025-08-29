// Fill out your copyright notice in the Description page of Project Settings.


#include "CityWidget.h"

#include "CityStorageWidget.h"
#include "RelicInformationPanel.h"

//setup its child widgets
void UCityWidget::NativeConstruct()
{
	if (CityStorage)
	{
		CityStorage->Setup(this);
	}

	if (RelicInformationPanel)
	{
		RelicInformationPanel->Setup(this);
	}
}

UCityStorageWidget* UCityWidget::GetCityStorage() const
{
	return CityStorage;
}


URelicInformationPanel* UCityWidget::GetRelicInformationPanel() const
{
	return RelicInformationPanel;
}

