// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestBoxWidget.generated.h"

class URequestsWidget;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URequestBoxWidget : public UUserWidget
{
	GENERATED_BODY() 

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetMainText(const FText& Text);

	UFUNCTION()
	void SetRequestID(const uint8 InRequestID);

	UFUNCTION()
	void SetRequestsWidget(URequestsWidget* InRequestsWidget);

protected:
	UFUNCTION()
	void OnButtonClicked();
	
	UPROPERTY(meta=(BindWidget))
	UButton* Button = nullptr;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MainText = nullptr;

	UPROPERTY()
	URequestsWidget* RequestsWidget = nullptr;

	UPROPERTY()
	uint8 RequestID = 0;
};
