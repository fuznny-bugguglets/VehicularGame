// Fill out your copyright notice in the Description page of Project Settings.


#include "NewHUDWidget.h"
#include "Vehicle.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UNewHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//rotate the speedo based on speed
	SpeedIndicator->SetRenderTransformAngle(GetVehicle()->GetSpeed() * 1.5f);
	
}

AVehicle* UNewHUDWidget::GetVehicle()
{
	//if we have it already, return it
	if (Vehicle)
	{
		return Vehicle;
	}

	//otherwise, find it
	Vehicle = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
	return Vehicle;
}
