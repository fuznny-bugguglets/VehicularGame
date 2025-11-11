// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Upgrades.h"
#include "Components/TextBlock.h"
#include "UpgradeButtonWidget.generated.h"

class UMechanicWidget;
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

	UFUNCTION()
	void SetupFromID(uint8 IncomingID);

	UFUNCTION()
	void SetMechanicWidget(UMechanicWidget* IncomingPtr);

	UFUNCTION()
	void RefreshDisplay();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ButtonText = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	FColor LockedColour = FColor::Purple;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	FColor UnaffordableColour = FColor::Purple;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	FColor AffordableColour = FColor::Purple;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	FColor PurchasedColour = FColor::Purple;

	UPROPERTY()
	uint8 UpgradeID = 255;

	UFUNCTION()
	UUpgradeSubsystem* GetUpgradeSubsystem();

	UPROPERTY()
	UUpgradeSubsystem* UpgradeSubsystem = nullptr;

	UPROPERTY()
	UMechanicWidget* MechanicWidget = nullptr;
	
	
};
