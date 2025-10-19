// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceTier Tier;

	FItem()
	{
		BuyPrice = 0;
		SellPrice = 0;
		Weight = 0.0f;
		Type = EResourceType::NULLRESOURCE;
		Tier = EResourceTier::NULLTier;
	}

	bool operator==(const FItem& Other) const
	{
		return false;
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
	static FItem& GetItemFromIndex(const uint8 Index);
	static uint8 GetIndexFromItem(const FItem& Item);

	static FItem& GetItemFromTypeAndTier(const EResourceType Type, const EResourceTier Tier);

	//setup in game instance
	//logic handled in blueprints to grab items from datatable
	UFUNCTION(BlueprintImplementableEvent)
	void SetupItemsFromDataTable();
	UFUNCTION(BlueprintCallable)
	void AddItem(const FItem& NewItem);

private:

	//used to store all existent items
	static TArray<FItem> Items;
};
