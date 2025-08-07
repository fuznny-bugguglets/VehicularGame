// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ruin.generated.h"

UCLASS()
class VEHICULARGAME_API ARuin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UStaticMeshComponent* RuinMeshCommon;
	UStaticMeshComponent* RuinMeshUncommon;
	UStaticMeshComponent* RuinMeshRare;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	//how much the ruin starts with
	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	int32 StartingResourceAmount = 1;

	//how much the ruin currently as
	int32 ResourceAmount = 0;
};
