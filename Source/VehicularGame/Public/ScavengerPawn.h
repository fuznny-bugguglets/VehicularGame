// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ScavengerPawn.generated.h"

class AAIController;
UCLASS()

class VEHICULARGAME_API AScavengerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AScavengerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AAIController* AIController = nullptr;

	//makes the scavenger visible and ticking
	void SetActive();
	void SetActive(const FVector& SpawnPos);

	//hides the scavenger and stops it from ticking
	void SetInactive();

	void LogError(const FString& ErrorMessage);

};
