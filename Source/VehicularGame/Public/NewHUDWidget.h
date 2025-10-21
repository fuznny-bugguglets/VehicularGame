// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewHUDWidget.generated.h"

class AVehicle;
class UImage;

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UNewHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UPROPERTY(meta=(BindWidget))
	UImage* SpeedIndicator = nullptr;
	
private:
	UPROPERTY()
	AVehicle* Vehicle = nullptr;
	
	UFUNCTION()
	AVehicle* GetVehicle();
	
	
};
