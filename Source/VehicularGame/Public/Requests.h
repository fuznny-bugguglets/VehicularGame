// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypes.h"
#include "Requests.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ERequestType : uint8
{
	CollectRequest UMETA(DisplayName = "Collect Request"),
	KillRequest UMETA(DisplayName = "KillRequest"),
	POIDiscoveryRequest UMETA(DisplayName = "DisplayName")
};


USTRUCT(BlueprintType)
struct FRequest : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FText Name;

	UPROPERTY()
	ERequestType RequestType;

	//Used for Collect, POI Discovery 
	UPROPERTY()
	EResourceTier Tier;

	//Used for Collect
	UPROPERTY()
	EResourceTier ItemType;

	//Used for Collect, Kill
	UPROPERTY()
	int32 Count;

	bool operator==(const FRequest& Other) const
	{
		return Name.ToString() == Other.Name.ToString();
	}
};

UCLASS(Blueprintable, BlueprintType)
class VEHICULARGAME_API URequestsManager : public UObject
{
	GENERATED_BODY()

public:
	static FRequest& GetRequestFromIndex(const uint8 Index);
	static uint8 GetIndexFromRequest(const FRequest& Request);

	//setup in game instance
	//logic handled in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void SetupItemsFromDataTable();
	UFUNCTION(BlueprintCallable)
	void AddRequest(const FRequest& Request);
	UFUNCTION(BlueprintCallable)
	void ClearRequests();


	static const TArray<FRequest>& GetRequests();
private:
	//stores all requests
	static TArray<FRequest> Requests;
	
};
