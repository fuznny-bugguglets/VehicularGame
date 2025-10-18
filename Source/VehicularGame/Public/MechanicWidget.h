// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeButtonWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Upgrades.h"
#include "Components/TextBlock.h"
#include "MechanicWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UMechanicWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void DisplayUpgradeInformation(uint8 UpgradeID);

protected:
	UPROPERTY(EditAnywhere, Category="Classes", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUpgradeButtonWidget> UpgradeButtonClass;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TurretTreeLevel1;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TurretTreeLevel2;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TurretTreeLevel3;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TurretTreeLevel4;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TurretTreeLevel5;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BumperTreeLevel1;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BumperTreeLevel2;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BumperTreeLevel3;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BumperTreeLevel4;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* BumperTreeLevel5;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CrewTreeLevel1;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CrewTreeLevel2;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CrewTreeLevel3;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CrewTreeLevel4;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CrewTreeLevel5;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CarTreeLevel1;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CarTreeLevel2;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CarTreeLevel3;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CarTreeLevel4;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* CarTreeLevel5;

	UFUNCTION()
	UHorizontalBox* GetUpgradeTree(EUpgradeTree UpgradeTree, uint8 Index);

	
	
};
