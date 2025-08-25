// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DevToolsWidget.generated.h"

class AVehicularGameState;
class AVehicularGameMode;
class AVehicle;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UDevToolsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

	UFUNCTION(BlueprintCallable)
	void GiveInfiniteResources();

	UFUNCTION(BlueprintCallable)
	void ForceSpawnEnemyWave();
	UFUNCTION(BlueprintCallable) 
	void PauseEnemyWaves();
	UFUNCTION(BlueprintCallable)
	void ResumeEnemyWaves();
	
	UFUNCTION(BlueprintCallable)
	void InfiniteHealth();

	UFUNCTION(BlueprintCallable)
	void TeleportToPOI();

	UFUNCTION(BlueprintCallable)
	void IncreaseDifficulty();
	UFUNCTION(BlueprintCallable)
	void DecreaseDifficulty();

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth();
	UFUNCTION(BlueprintCallable)
	void DecreaseHealth();

	//adding resources
	UFUNCTION()
	void OnAddCommonResource();

	UFUNCTION()
	void OnAddUncommonResource();

	UFUNCTION()
	void OnAddRareResource();

protected:
	//buttons
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AddCommonResourcesButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AddUncommonResourcesButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AddRareResourcesButton;

	virtual void NativeConstruct() override;


private:
	UPROPERTY()
	AVehicularGameMode* VehicularGameMode = nullptr;

	UPROPERTY()
	AVehicularGameState* VehicularGameState = nullptr;
	
	UPROPERTY()
	AVehicle* Vehicle = nullptr;
};
