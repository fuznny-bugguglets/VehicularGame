// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemButtonWidget.h"

#include "CityWidget.h"
#include "RelicInformationPanel.h"

//default constructor, sets up clilck delegate
void UItemButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!Button)
	{
		return;
	}

	Button->OnClicked.AddDynamic(this, &UItemButtonWidget::OnButtonClicked);
}


//called when the user clicks on the button
//displays information about this item on the relic information panel
void UItemButtonWidget::OnButtonClicked()
{
	if (CityWidget)
	{
		if (CityWidget->GetRelicInformationPanel())
		{
			CityWidget->GetRelicInformationPanel()->DisplayItemInformation(MyItemID, bSell);
		}
	}
	
}


void UItemButtonWidget::SetItemID(uint8 InID)
{
	MyItemID = InID;
}


void UItemButtonWidget::SetText(const FText& InMainText, const FText& InSubText)
{
	//

	SetMainText(InMainText);
	SetSubText(InSubText);
}

void UItemButtonWidget::SetMainText(const FText& InText)
{
	if(!MainText)
	{
		return;
	}

	MainText->SetText(InText);
}

void UItemButtonWidget::SetSubText(const FText& InText)
{
	if(!SubText)
	{
		return;
	}

	SubText->SetText(InText);
}

void UItemButtonWidget::SetBuySellType(bool bShouldSell)
{
	bSell = bShouldSell;
}

uint8 UItemButtonWidget::GetItemID() const
{
	return MyItemID;
}
