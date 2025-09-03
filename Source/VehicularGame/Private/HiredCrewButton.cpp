// Fill out your copyright notice in the Description page of Project Settings.


#include "HiredCrewButton.h"

#include "CityWidget.h"
#include "CrewInformationPanel.h"

void UHiredCrewButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UHiredCrewButton::OnButtonClicked);
}

void UHiredCrewButton::OnButtonClicked()
{
	//if its null, ignore
	if (ButtonID == 255)
	{
		return;
	}
	
	if (CityWidget)
	{
		if (CityWidget->GetCrewInformationPanel())
		{
			CityWidget->GetCrewInformationPanel()->DisplayCrewInformation(ButtonID, true);
		}
	}
}
