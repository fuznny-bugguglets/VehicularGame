// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Upgrades.h"
#include "Crew.generated.h"

USTRUCT(BlueprintType)
struct FCrewPassive
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUpgradeType UpgradeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Multiplier;
};

USTRUCT(BlueprintType)
struct FCrew : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCrewPassive> Passives;

	bool operator==(const FCrew& Other) const
	{
		FString MyStringName = this->Name.ToString();
		FString TheirStringName = Other.Name.ToString();

		return MyStringName == TheirStringName;
	}
	
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VEHICULARGAME_API UCrewManager : public UObject
{
	GENERATED_BODY()
public:

	static FCrew& GetCrewFromIndex(const uint8 Index);
	static uint8 GetIndexFromCrew(const FCrew& InCrew);


	//setup in game instance
	//logic handled in blueprints to grab crew from data table
	UFUNCTION(BlueprintImplementableEvent)
	void SetupCrewFromDataTable();

	UFUNCTION(BlueprintCallable)
	void AddCrew(const FCrew& NewCrew);

private:
	//used to store all types of crew
	static TArray<FCrew> Crew;
};
