// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsWidget.h"

#include "Requests.h"
#include "RequestBoxWidget.h"
#include "Components/ScrollBox.h"

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

		RequestsScrollBox->AddChild(RequestBox);
	}
}
