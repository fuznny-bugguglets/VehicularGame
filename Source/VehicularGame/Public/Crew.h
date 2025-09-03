// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Crew.generated.h"

UENUM()
enum class EAttributeName : uint8
{
	SharpShooter,
	Marathoner,
	Storyteller,
	Grumpy

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
	TArray<EAttributeName> Attributes;
	
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VEHICULARGAME_API UCrewManager : public UObject
{
	GENERATED_BODY()
public:

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
