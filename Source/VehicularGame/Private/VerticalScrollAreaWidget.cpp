// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalScrollAreaWidget.h"

#include "InventorySubsystem.h"
#include "ItemButtonWidget.h"

//default constructor
void UVerticalScrollAreaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

//called by CityWidget at the start
void UVerticalScrollAreaWidget::Setup(UCityWidget* InCity)
{
	CityWidget = InCity;
}

//creates a new button inside the scroll (called by subclasses)
void UVerticalScrollAreaWidget::AddItemBlock(const uint8 ID, const FText& MainText, const FText& SubText)
{
	if (!ItemButtonWidgetClass)
	{
		return;
	}

	UItemButtonWidget* NewButton = CreateWidget<UItemButtonWidget>(GetWorld(), ItemButtonWidgetClass);
	if (!NewButton)
	{
		return;
	}

	//setup button with information ID and text to display
	NewButton->Setup(CityWidget);
	NewButton->SetText(MainText, SubText);
	NewButton->SetItemID(ID);

	if (ScrollyBox)
	{
		ScrollyBox->AddChild(NewButton);
	}

	Buttons.Add(NewButton);
	
}

void UVerticalScrollAreaWidget::UpdateButtons()
{
	for (auto Button : Buttons)
	{
		if (!Button)
		{
			continue;
		}
		
		UpdateButton(Button);
	}
}

void UVerticalScrollAreaWidget::UpdateButton(uint8 ItemID)
{
	//looks for the buttons
	for (auto Button : Buttons)
	{
		if (!Button)
		{
			continue;
		}
		
		//is this the button we are looking for?
		if (Button->GetItemID() == ItemID)
		{
			UpdateButton(Button);
			return;
		}
	}
}

void UVerticalScrollAreaWidget::UpdateButton(UItemButtonWidget* Button)
{
	//pure virtual function
	//but they don't exist in unreal
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("UpdateButton in VerticalScrollAreaWidget should not of run"));
	return;
}

void UVerticalScrollAreaWidget::CreateItemBlock(uint8 ID) 
{
	//pure virtual function
	//but they don't exist in unreal
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CreateItemBlock in VerticalScrollAreaWidget should not of run"));
	return;
}

bool UVerticalScrollAreaWidget::DoesItemBlockExist(uint8 ID)
{
	for (auto Button : Buttons)
	{
		if (Button)
		{
			//does the item match the ID
			if (Button->GetItemID() == ID)
			{
				return true;
			}
		}
	}

	return false;
}

UInventorySubsystem* UVerticalScrollAreaWidget::GetInventory()
{
	if (!Inventory)
	{
		Inventory = Cast<UInventorySubsystem>(GetGameInstance()->GetSubsystem<UInventorySubsystem>());
	}

	return Inventory;
}
