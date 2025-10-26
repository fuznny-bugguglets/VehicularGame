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

	//Used for Collect Request and POI Discovery Request
	UPROPERTY()
	EResourceTier Tier;

	//Used for Collect Request
	UPROPERTY()
	EResourceTier ItemType;
};

UCLASS(Blueprintable, BlueprintType)
class VEHICULARGAME_API URequestsManager : public UObject
{
	GENERATED_BODY()

public:

	//setup in game instance
	//logic handled in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void SetupItemsFromDataTable();
	UFUNCTION(BlueprintCallable)
	void AddRequest(const FRequest& Request);
	UFUNCTION(BlueprintCallable)
	void ClearRequests();
private:
	//stores all requests
	static TArray<FRequest> Requests;
	
};
