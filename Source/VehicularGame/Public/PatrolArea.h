// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolArea.generated.h"

class AEnemyCharacter;

UCLASS()
class VEHICULARGAME_API APatrolArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called by an enemy when they need a new location to travel to
	FVector GetRandomPatrolPoint() const;


private:
	//all the areas an enemy could go to while patrolling
	UPROPERTY(EditInstanceOnly, Category="Patrol Points", meta=(AllowPrivateAccess="true"))
	TArray<AActor*> PatrolPoints;

	//the amount of each enemy to spawn
	UPROPERTY(EditInstanceOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	uint8 SmallEnemiesToSpawn;
	UPROPERTY(EditInstanceOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	uint8 MediumEnemiesToSpawn;
	UPROPERTY(EditInstanceOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	uint8 LargeEnemiesToSpawn;
	UPROPERTY(EditInstanceOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	uint8 HugeEnemiesToSpawn;


	//the classes of enemies to spawn
	UPROPERTY(EditDefaultsOnly, Category="Enemies", meta=(AllowPrivateAccess="true")) 
	TSubclassOf<AEnemyCharacter> SmallEnemyClass;
	UPROPERTY(EditDefaultsOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AEnemyCharacter> MediumEnemyClass;
	UPROPERTY(EditDefaultsOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AEnemyCharacter> LargeEnemyClass;
	UPROPERTY(EditDefaultsOnly, Category="Enemies", meta=(AllowPrivateAccess="true"))
	TSubclassOf<AEnemyCharacter> HugeEnemyClass;

};
