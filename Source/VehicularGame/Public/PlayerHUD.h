// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class AVehicularGameState;
class AVehicle;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	explicit UPlayerHUD(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	FText FetchSpeedText();

	UFUNCTION(BlueprintCallable)
	FText FetchEngineStateText();

	UFUNCTION(BlueprintCallable)
	FText FetchHandbreakStatusText();

	UFUNCTION(BlueprintCallable)
	FText FetchNumericalHPText();

	UFUNCTION(BlueprintCallable)
	float FetchHPPercentage();

	UFUNCTION(BlueprintCallable)
	float FetchDPMPercentage();
	

	
	
private:
	void LogError(const FString& ErrorMessage) const;

	UPROPERTY()
	AVehicle* PlayerRef = nullptr;

	UPROPERTY()
	AVehicularGameState* GameStateRef = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DifficultyMax = 30.0f;

};
