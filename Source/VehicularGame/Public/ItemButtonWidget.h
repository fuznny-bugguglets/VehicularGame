// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ItemButtonWidget.generated.h"

class UCityWidget;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UItemButtonWidget : public UUserWidget
{
	GENERATED_BODY()  

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void Setup(UCityWidget* InCity);

	uint8 GetItemID() const;
	
	void SetItemID(uint8 InID);
	void SetBuySellType(bool bShouldSell);
	void SetText(const FText& InMainText, const FText& InSubText);
	void SetMainText(const FText& InText);
	void SetSubText(const FText& InText);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* Button = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* MainText = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* SubText = nullptr;

	UPROPERTY()
	UCityWidget* CityWidget = nullptr;

	UPROPERTY()
	uint8 MyItemID = 255;

	UPROPERTY()
	bool bSell = false;

	UFUNCTION()
	void OnButtonClicked();
	
};
