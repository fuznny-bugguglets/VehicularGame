// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsWidget.h"

#include "Requests.h"
#include "RequestBoxWidget.h"
#include "RequestsSubsystem.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void URequestsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!RequestBoxWidgetClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No Request Box Widget Set!");
		return;
	}

	for (const FRequest& Request : URequestsManager::GetRequests())
	{
		URequestBoxWidget* RequestBox = CreateWidget<URequestBoxWidget>(GetWorld(), RequestBoxWidgetClass);

		if (!RequestBox)
		{
			return;
		}

		RequestBox->SetMainText(Request.Name);
		RequestBox->SetRequestID(URequestsManager::GetIndexFromRequest(Request));
		RequestBox->SetRequestsWidget(this);

		RequestsScrollBox->AddChild(RequestBox);
	}

	HideInfoPanel();
}

void URequestsWidget::OnRequestSelected(uint8 RequestID)
{
	ShowInfoPanel(RequestID);
}

void URequestsWidget::HideInfoPanel()
{
	NameText->SetText(FText::GetEmpty());
	DescriptionText->SetText(FText::GetEmpty());
	RedeemButton->SetVisibility(ESlateVisibility::Hidden);
}

void URequestsWidget::ShowInfoPanel(const uint8 RequestID)
{
	FRequest& Request = URequestsManager::GetRequestFromIndex(RequestID);

	URequestsSubsystem* RequestsSubsystem = GetGameInstance()->GetSubsystem<URequestsSubsystem>();
	if (!RequestsSubsystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			"Failed to find requests subsystem in requests widget");
		return;
	}
	
	NameText->SetText(Request.Name);

	FString DescriptionString;
	DescriptionString.Append(Request.Description.ToString());
	DescriptionString.Append("\n\n");
	
	DescriptionString.Append("Reward: $");
	DescriptionString.AppendInt(Request.MoneyReward);
	DescriptionString.Append("\n");

	
	if (Request.RequestType == ERequestType::KillRequest)
	{
		DescriptionString.Append("Killed: ");
		DescriptionString.AppendInt(RequestsSubsystem->GetEnemiesKilled());
		DescriptionString.Append("/");
		DescriptionString.AppendInt(Request.Count);
	}
	
	DescriptionText->SetText(FText::FromString(DescriptionString));
	
	//has it been redeemed already?
	if (RequestsSubsystem->GetRequestRedeemedStatus(RequestID))
	{
		RedeemButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		RedeemButton->SetVisibility(ESlateVisibility::Visible);
		//has it been achieved?
		if (RequestsSubsystem->GetRequestAchievementStatus(RequestID))
		{
			RedeemButton->SetBackgroundColor(FColor::Green);
		}
		else
		{
			RedeemButton->SetBackgroundColor(FColor::Red);
		}
	}
}

