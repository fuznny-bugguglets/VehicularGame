// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeButtonWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Upgrades.h"
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

protected:
	UPROPERTY(EditAnywhere, Category="Classes", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UUpgradeButtonWidget> UpgradeButtonClass;
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* TurretTreeLevel1;

	UFUNCTION()
	UHorizontalBox* GetUpgradeTree(EUpgradeTree UpgradeTree, uint8 Index);

	
	
};
