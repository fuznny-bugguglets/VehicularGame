// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewHUDWidget.generated.h"

class UProgressBar;
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

	UFUNCTION(BlueprintImplementableEvent)
	void EnableRadioAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void DisableRadioAnimation();

	UFUNCTION(BlueprintCallable)
	void EnableExtractionAnimation();
	UFUNCTION(BlueprintCallable)
	void DisableExtractionAnimation();
	
protected:
	UPROPERTY(EditAnywhere, Category="Speedo", meta=(AllowPrivateAccess="true"))
	float SpeedoLerpSpeed = 50.0f;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SpeedIndicator = nullptr;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* PB_Health = nullptr;
	
private:
	UFUNCTION()
	void UpdateSpeedo(float DeltaTime);

	UFUNCTION()
	void UpdateHealthBar();
	
	UPROPERTY()
	AVehicle* Vehicle = nullptr;
	
	UFUNCTION()
	AVehicle* GetVehicle();
	
	
};
