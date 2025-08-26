// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ruin.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType Tier;

	FItem()
	{
		Value = 0;
		Weight = 0.0f;
		Tier = EResourceType::NULLRESOURCE;
	}
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VEHICULARGAME_API UItemManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupItemsFromStruct();

	UFUNCTION(BlueprintCallable)
	void AddItem(const FItem& NewItem);

private:

	//used to store
	TArray<FItem> Items;
};
