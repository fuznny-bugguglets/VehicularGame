// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestBoxWidget.h"

#include "Requests.h"
#include "RequestsSubsystem.h"
#include "RequestsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URequestBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &URequestBoxWidget::OnButtonClicked);
}


void URequestBoxWidget::SetMainText(const FText& Text)
{
	MainText->SetText(Text);
}

void URequestBoxWidget::SetRequestID(const uint8 InRequestID)
{
	RequestID = InRequestID;

	//has the quest been completed?
	if (GetGameInstance()->GetSubsystem<URequestsSubsystem>()->GetRequestAchievementStatus(InRequestID))
	{
		Button->SetBackgroundColor(FColor::Emerald);
	}
}

void URequestBoxWidget::SetRequestsWidget(URequestsWidget* InRequestsWidget)
{
	RequestsWidget = InRequestsWidget;
}


void URequestBoxWidget::OnButtonClicked()
{
	if(!RequestsWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			"No request widget set for Request Box Widget");
		return;
	}

	RequestsWidget->OnRequestSelected(RequestID);
}

