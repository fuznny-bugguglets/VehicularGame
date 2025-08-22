// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DevToolsWidget.generated.h"

class AVehicle;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UDevToolsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void GiveInfiniteResources();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	AVehicle* Vehicle;
};
