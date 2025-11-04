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

	//force the enemies to spawn
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	//spawn enemies and set them to patrol
	for (uint8 i = 0; i < SmallEnemiesToSpawn; i++)
	{
		if (AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(SmallEnemyClass, GetActorLocation(), GetActorRotation(), SpawnParameters))
		{
			Enemy->BeginPatrol(this);
		}
	}
	for (uint8 i = 0; i < MediumEnemiesToSpawn; i++)
	{
		if (AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(MediumEnemyClass, GetActorLocation(), GetActorRotation(), SpawnParameters))
		{
			Enemy->BeginPatrol(this);
		}
		
	}
	for (uint8 i = 0; i < LargeEnemiesToSpawn; i++)
	{
		if (AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(LargeEnemyClass, GetActorLocation(), GetActorRotation(), SpawnParameters))
		{
			Enemy->BeginPatrol(this);
		}
	}
	for (uint8 i = 0; i < HugeEnemiesToSpawn; i++)
	{
		if (AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(HugeEnemyClass, GetActorLocation(), GetActorRotation(), SpawnParameters))
		{
			Enemy->BeginPatrol(this);
		}
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

