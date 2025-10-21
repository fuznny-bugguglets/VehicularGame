// Fill out your copyright notice in the Description page of Project Settings.


#include "NewHUDWidget.h"
#include "Vehicle.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UNewHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateSpeedo(InDeltaTime);
	UpdateHealthBar();
	
}

void UNewHUDWidget::UpdateSpeedo(float DeltaTime)
{
	//get current and target angle
	const float CurrentAngle = SpeedIndicator->GetRenderTransformAngle();
	const float TargetAngle = FMath::Abs(GetVehicle()->GetSpeed() * 1.5f);

	//get the angle increase this frame
	const float AngleIncrease = SpeedoLerpSpeed * DeltaTime;

	//get the difference between target and current angles
	const float AngleDifference = TargetAngle-CurrentAngle;

	//figure out the direction
	bool bIsIncreasing = false;
	if (AngleDifference > 0)
		bIsIncreasing = true;
	
	//will the increase be bigger than the actual difference?
	if (AngleIncrease >=  FMath::Abs(AngleDifference))
	{
		//set the speedo to the target
		SpeedIndicator->SetRenderTransformAngle(TargetAngle);
	}
	else
	{
		//move the speedo closer to the target
		if (bIsIncreasing)
		{
			SpeedIndicator->SetRenderTransformAngle(CurrentAngle + AngleIncrease);
		}
		else
		{
			SpeedIndicator->SetRenderTransformAngle(CurrentAngle - AngleIncrease);
		}
	}
}

void UNewHUDWidget::UpdateHealthBar()
{
	const float Percent = GetVehicle()->GetHealth() / GetVehicle()->GetMaxHealth();
	PB_Health->SetPercent(Percent);
}

void UNewHUDWidget::EnableExtractionAnimation()
{
	
}

void UNewHUDWidget::DisableExtractionAnimation()
{
	
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
