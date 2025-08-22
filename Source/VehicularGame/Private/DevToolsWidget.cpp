// Fill out your copyright notice in the Description page of Project Settings.


#include "DevToolsWidget.h"

#include "Vehicle.h"
#include "Kismet/GameplayStatics.h"

void UDevToolsWidget::NativeConstruct()
{
	Vehicle = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
}


void UDevToolsWidget::GiveInfiniteResources()
{
	if (!Vehicle)
	{
		return;
	}

	Vehicle->SetCommonLootCount(99999);
	Vehicle->SetUncommonLootCount(99999);
	Vehicle->SetRareLootCount(99999);
}
