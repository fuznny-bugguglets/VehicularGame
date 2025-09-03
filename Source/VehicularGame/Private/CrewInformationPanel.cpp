// Fill out your copyright notice in the Description page of Project Settings.


#include "CrewInformationPanel.h"

#include "Crew.h"
#include "Components/TextBlock.h"

void UCrewInformationPanel::NativeConstruct()
{
	Super::NativeConstruct();

	InteractionButton->OnClicked.AddDynamic(this, &UCrewInformationPanel::OnButtonClicked);
}

void UCrewInformationPanel::DisplayCrewInformation(uint8 ID, bool _bIsHired)
{
	CrewID = ID;
	bIsHired = _bIsHired; 

	FCrew& Crew = UCrewManager::GetCrewFromIndex(ID);

	InteractionButton->SetVisibility(ESlateVisibility::Visible);

	MainText->SetText(Crew.Name);
	SubText->SetText(Crew.Description);

	if (bIsHired)
	{
		InteractionButtonMainText->SetText(FText::FromString("Lay Off"));
	}
	else
	{
		InteractionButtonMainText->SetText(FText::FromString("Hire"));
	}
}

void UCrewInformationPanel::HideDisplay()
{
	InteractionButton->SetVisibility(ESlateVisibility::Hidden);
	MainText->SetText(FText::GetEmpty());
	SubText->SetText(FText::GetEmpty());
	InteractionButtonMainText->SetText(FText::GetEmpty());
}


void UCrewInformationPanel::OnButtonClicked()
{
	if (CityWidget)
	{
		CityWidget->HireCrewMember(CrewID);
	}
}
