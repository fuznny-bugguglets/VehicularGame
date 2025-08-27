// Fill out your copyright notice in the Description page of Project Settings.


#include "CityStorageWidget.h"
#include "InventorySubsystem.h"

void UCityStorageWidget::NativeConstruct()
{
	UInventorySubsystem* InventorySystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
}
