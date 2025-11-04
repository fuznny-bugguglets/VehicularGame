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

	//check we have patrol points
	checkf(PatrolPoints.Num() > 0, TEXT("There is a Patrol Area without any Patrol Points. Look for %s and fix it up"), *GetActorNameOrLabel())

	//spawn enemies and set them to patrol
	for (uint8 i = 0; i < SmallEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(SmallEnemyClass, GetActorLocation(), GetActorRotation())->BeginPatrol(this);
	}
	for (uint8 i = 0; i < MediumEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(MediumEnemyClass, GetActorLocation(), GetActorRotation())->BeginPatrol(this);
	}
	for (uint8 i = 0; i < LargeEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(LargeEnemyClass, GetActorLocation(), GetActorRotation())->BeginPatrol(this);
	}
	for (uint8 i = 0; i < HugeEnemiesToSpawn; i++)
	{
		GetWorld()->SpawnActor<AEnemyCharacter>(HugeEnemyClass, GetActorLocation(), GetActorRotation())->BeginPatrol(this);
	}
}

// Called every frame
void APatrolArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APatrolArea::GetRandomPatrolPoint() const
{
	const int32 RandomIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);

	return PatrolPoints[RandomIndex]->GetActorLocation();
}

