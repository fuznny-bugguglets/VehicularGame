// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

UCLASS()
class VEHICULARGAME_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Loot Getters and Setters
	int32 GetCommonLootCount() const;
	int32 GetUncommonLootCount() const;
	int32 GetRareLootCount() const;

	void ResetAllLoot();
	void SetCommonLootCount(int32 NewValue);
	void SetUncommonLootCount(int32 NewValue);
	void SetRareLootCount(int32 NewValue);
 
private:
	int32 CommonLootCount = 0;
	int32 UncommonLootCount = 0;
	int32 RareLootCount = 0;

};
