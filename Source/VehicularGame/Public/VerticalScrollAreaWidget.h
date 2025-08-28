// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemButtonWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "VerticalScrollAreaWidget.generated.h"

/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UVerticalScrollAreaWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void AddItemBlock(const FText& MainText, const FText& SubText);

protected:
	//where the item buttons will appear
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ScrollyBox = nullptr;

	//where the information will appear
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InformationMainText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InformationSubText = nullptr;

	//where the button will be (either sell or buy based on children)
	UPROPERTY(meta = (BindWidget))
	UButton* InteractionButton = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionButtonMainText = nullptr;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionButtonSubText = nullptr;
	

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemButtonWidget> ItemButtonWidgetClass;

	
};
