// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RequestsWidget.generated.h"

class UCityWidget;
class UButton;
class URequestBoxWidget;
class UScrollBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URequestsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void OnRequestSelected(uint8 RequestID);

	UFUNCTION()
	void OnRedeemButton();

	UFUNCTION()
	void SetCityWidget(UCityWidget* In);

protected:
	UFUNCTION()
	void HideInfoPanel();

	UFUNCTION()
	void ShowInfoPanel(const uint8 RequestID);
	
	UPROPERTY(meta=(BindWidget))
	UScrollBox* RequestsScrollBox = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DescriptionText = nullptr;

	UPROPERTY(meta=(BindWidget))
	UButton* RedeemButton = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess="true"))
	TSubclassOf<URequestBoxWidget> RequestBoxWidgetClass;

	UPROPERTY()
	uint8 SelectedRequestID = 0;

	UPROPERTY()
	UCityWidget* CityWidget = nullptr;

private:
	UPROPERTY()
	TMap<uint8, URequestBoxWidget*> RequestBoxes;
};
