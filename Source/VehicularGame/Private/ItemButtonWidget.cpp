// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButtonWidget.h"

#include "CityWidget.h"
#include "RelicInformationPanel.h"
#include "VerticalScrollAreaWidget.h"

void UItemButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!Button)
	{
		return;
	}

	Button->OnClicked.AddDynamic(this, &UItemButtonWidget::OnButtonClicked);
}

void UItemButtonWidget::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}


void UItemButtonWidget::OnButtonClicked()
{
	if (CityWidget)
	{
		if (CityWidget->GetRelicInformationPanel())
		{
			CityWidget->GetRelicInformationPanel()->DisplayItemInformation(MyItemID);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No relic info widget"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No city widget"));
	}
	
}


void UItemButtonWidget::SetItemID(uint8 InID)
{
	MyItemID = InID;
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


