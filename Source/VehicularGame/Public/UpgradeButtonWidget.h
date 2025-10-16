// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Upgrades.h"
#include "UpgradeButtonWidget.generated.h"

class UButton;
class UUpgradeSubsystem;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UUpgradeButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button = nullptr;

	UPROPERTY(EditAnywhere)
	EUpgradeType UpgradeType;

	UPROPERTY(EditAnywhere)
	float UpgradeValue;

	UFUNCTION()
	UUpgradeSubsystem* GetUpgradeSubsystem();

	UPROPERTY()
	UUpgradeSubsystem* UpgradeSubsystem = nullptr;
	
};
