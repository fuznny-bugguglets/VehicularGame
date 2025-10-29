// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestsWidget.generated.h"

class URequestBoxWidget;
class UScrollBox;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URequestsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* RequestsScrollBox = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<URequestBoxWidget> RequestBoxWidgetClass;
};
