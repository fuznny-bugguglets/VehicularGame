// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolArea.h"
#include "EnemyCharacter.h"

// Sets default values
APatrolArea::APatrolArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolArea::BeginPlay()
{
	Super::BeginPlay();

	//spawn enemies and set them to patrol
	for (uint8 i = 0; i < SmallEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(SmallEnemyClass, GetActorLocation(), GetActorRotation())->SetEnemyState(EEnemyState::PATROLLING);
	}
	for (uint8 i = 0; i < MediumEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(MediumEnemyClass, GetActorLocation(), GetActorRotation())->SetEnemyState(EEnemyState::PATROLLING);
	}
	for (uint8 i = 0; i < LargeEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(LargeEnemyClass, GetActorLocation(), GetActorRotation())->SetEnemyState(EEnemyState::PATROLLING);
	}
	for (uint8 i = 0; i < HugeEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(HugeEnemyClass, GetActorLocation(), GetActorRotation())->SetEnemyState(EEnemyState::PATROLLING);
	}
}

// Called every frame
void APatrolArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

