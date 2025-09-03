// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewButtonWidget.h"

#include "CityWidget.h"
#include "CrewInformationPanel.h"

void UCrewButtonWidget::NativeConstruct()
{
	if (!Button)
	{
		return;
	}

	Button->OnClicked.AddDynamic(this, &UCrewButtonWidget::OnButtonClicked);
}

void UCrewButtonWidget::OnButtonClicked()
{
	if (CityWidget)
	{
		if (CityWidget->GetCrewInformationPanel())
		{
			CityWidget->GetCrewInformationPanel()->DisplayCrewInformation(ButtonID, false);
		}
	}
}

