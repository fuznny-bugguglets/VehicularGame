// Fill out your copyright notice in the Description page of Project Settings.


#include "Ruin.h"

// Sets default values
ARuin::ARuin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup default meshes
	//RuinMeshCommon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ruin Mesh Common"));
	//RuinMeshUncommon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ruin Mesh Unommon"));
	//RuinMeshRare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ruin Mesh Ruin"));

}

// Called when the game starts or when spawned
void ARuin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARuin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

