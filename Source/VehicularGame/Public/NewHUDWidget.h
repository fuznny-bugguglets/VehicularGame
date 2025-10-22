// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewHUDWidget.generated.h"

class AVehicularGameState;
class UProgressBar;
class AVehicle;
class UImage;
class UTextBlock;

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

	UFUNCTION(BlueprintImplementableEvent)
	void EnableExtractionAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void DisableExtractionAnimation();

	UFUNCTION(BlueprintCallable)
	void EnableHandbrake();
	UFUNCTION(BlueprintCallable)
	void DisableHandbrake();

	void UpdateExtractionProgress(int32 CurrentCount, int32 InitCount);
	
protected:
	UPROPERTY(EditAnywhere, Category="Speedo", meta=(AllowPrivateAccess="true"))
	float SpeedoLerpSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "HazTrack", meta = (AllowPrivateAccess = "true"))
	float HazTrackMin = 0.0f;
	UPROPERTY(EditAnywhere, Category = "HazTrack", meta = (AllowPrivateAccess = "true"))
	float HazTrackMax = 15.0f;
	
	UPROPERTY(meta=(BindWidget))
	UImage* SpeedIndicator = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* T_Speedometer = nullptr;

	UPROPERTY(meta=(BindWidget))
	UProgressBar* PB_Health = nullptr;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_HazTrack = nullptr;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_POI_Extraction = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* I_Handbrake_Up = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* I_Handbrake_Down = nullptr;
	
private:
	UFUNCTION()
	void UpdateSpeedo(float DeltaTime);

	UFUNCTION()
	void UpdateHealthBar();

	UFUNCTION()
	void UpdateHazTrack();
	
	UPROPERTY()
	AVehicle* Vehicle = nullptr;

	UPROPERTY()
	AVehicularGameState* VGameState = nullptr;
	
	UFUNCTION()
	AVehicle* GetVehicle();

	UFUNCTION()
	AVehicularGameState* GetVGameState();
	
	
};
