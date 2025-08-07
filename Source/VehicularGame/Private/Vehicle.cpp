// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	//get the scavenger game state
	GetWorld()->GetGameState();
	
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Loot
int32 AVehicle::GetCommonLootCount() const
{
	return CommonLootCount;
}
int32 AVehicle::GetUncommonLootCount() const
{
	return UncommonLootCount;
}
int32 AVehicle::GetRareLootCount() const
{
	return RareLootCount;
}

void AVehicle::ResetAllLoot()
{
	SetCommonLootCount(0);
	SetUncommonLootCount(0);
	SetRareLootCount(0);
} 
void AVehicle::SetCommonLootCount(int32 NewValue)
{
	CommonLootCount = NewValue;
}
void AVehicle::SetUncommonLootCount(int32 NewValue)
{
	UncommonLootCount = NewValue;
}
void AVehicle::SetRareLootCount(int32 NewValue)
{
	RareLootCount = NewValue;
}







