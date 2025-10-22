// Fill out your copyright notice in the Description page of Project Settings.


#include "NewHUDWidget.h"
#include "Vehicle.h"
#include "VehicularGameState.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UNewHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateSpeedo(InDeltaTime);
	UpdateHealthBar();
	UpdateHazTrack();
	
}

void UNewHUDWidget::UpdateSpeedo(float DeltaTime)
{
	//set text
	int32 SpeedAsInt = GetVehicle()->GetSpeed();
	SpeedAsInt = FMath::Abs(SpeedAsInt);
	T_Speedometer->SetText(FText::FromString(FString::FromInt(SpeedAsInt)));

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

void UNewHUDWidget::UpdateHazTrack()
{
	//get the difficulty from the game state
	const float CurrentDifficulty = GetVGameState()->Difficulty;

	//figure out the percentage
	float Percentage = (CurrentDifficulty - HazTrackMin) / (HazTrackMax - HazTrackMin);
	Percentage = FMath::Clamp(Percentage, 0.0f, 1.0f);

	//apply to haz track
	PB_HazTrack->SetPercent(Percentage);
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

AVehicularGameState* UNewHUDWidget::GetVGameState()
{
	//if we have it already, return it
	if (VGameState)
	{
		return VGameState;
	}

	//otherwise, find it
	VGameState = Cast<AVehicularGameState>(UGameplayStatics::GetGameState(this));
	return VGameState;
}


void UNewHUDWidget::UpdateExtractionProgress(int32 CurrentCount, int32 InitCount)
{
	float Progress = (float)CurrentCount / (float)InitCount;

	PB_POI_Extraction->SetPercent(Progress);
}

void UNewHUDWidget::EnableHandbrake()
{
	I_Handbrake_Up->SetVisibility(ESlateVisibility::Visible);
	I_Handbrake_Down->SetVisibility(ESlateVisibility::Hidden);
}

void UNewHUDWidget::DisableHandbrake()
{
	I_Handbrake_Down->SetVisibility(ESlateVisibility::Visible);
	I_Handbrake_Up->SetVisibility(ESlateVisibility::Hidden);
}

