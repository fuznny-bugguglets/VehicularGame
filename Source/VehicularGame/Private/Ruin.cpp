// Fill out your copyright notice in the Description page of Project Settings.


#include "Ruin.h"
#include "VehicularGameInstance.h"
#include "VehicularSaveGame.h"
#include "Kismet/KismetSystemLibrary.h"

void LogError(const FString& ErrorMessage)
{
	//if we have the engine pointer, we print to the screen
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorMessage);
	}
	//otherwise, we print to the log
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorMessage);
	}
}

// Sets default values
ARuin::ARuin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup default meshes
	RuinMeshCommon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ruin Mesh Common"));
	RuinMeshUncommon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ruin Mesh Unommon"));
	RuinMeshRare = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ruin Mesh Rare"));

}

// Called when the game starts or when spawned
void ARuin::BeginPlay()
{
	Super::BeginPlay();

	//make sure the game instance is real
	if(!GetGameInstance())
	{
		LogError(TEXT("No game instance in ruin. (wtf???)"));
		return;
	}

	//get the vehicular game instance
	UVehicularGameInstance* VGameInstance = Cast<UVehicularGameInstance>(GetGameInstance());
	if(VGameInstance == nullptr)
	{
		LogError(TEXT("Failed to cast to VehicularGameInstance in Ruin"));
		return;
	}

	//get the save game object
	UVehicularSaveGame* SaveGameObject = VGameInstance->GetSaveGameObject();
	if(SaveGameObject == nullptr)
	{
		LogError(TEXT("Failed to get the save game object in Ruin"));
		return;
	}

	//tries to find information about the ruin in the save file
	int32* DataResourceAmount = SaveGameObject->RuinResourceData.Find(FName(*UKismetSystemLibrary::GetDisplayName(this)));
	
	//nothing was found in the save
	if(DataResourceAmount == nullptr)
	{
		//set the resource amount to the default
		ResourceAmount = StartingResourceAmount;
	}
	else
	{
		//set the resource amount found in the data
		ResourceAmount = *DataResourceAmount;
	}
	
}

// Called every frame
void ARuin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

