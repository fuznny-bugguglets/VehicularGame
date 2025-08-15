// Fill out your copyright notice in the Description page of Project Settings.


#include "Ruin.h"
#include "VehicularGameInstance.h"
#include "VehicularSaveGame.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void ARuin::LogError(const FString& ErrorMessage)
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
	ExtractionRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Extraction Ring Mesh"));
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Extraction Radius"));

	//setup attachments
	RuinMeshCommon->SetupAttachment(RootComponent);
	RuinMeshUncommon->SetupAttachment(RuinMeshCommon);
	RuinMeshRare->SetupAttachment(RuinMeshCommon);
	ExtractionRingMesh->SetupAttachment(RuinMeshCommon);
	SphereCollider->SetupAttachment(RuinMeshCommon);

	
	
	//disable collision on all meshes
	RuinMeshCommon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RuinMeshUncommon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RuinMeshRare->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	

	
}

// Called when the game starts or when spawned
void ARuin::BeginPlay()
{
	Super::BeginPlay();

	//hide all meshes
	RuinMeshCommon->SetVisibility(false);
	RuinMeshUncommon->SetVisibility(false);
	RuinMeshRare->SetVisibility(false);
	
	//set meshes based on type
	switch (ResourceType)
	{
	case EResourceType::COMMON:
		ExtractionRingMesh->SetMaterial(0, CommonMaterial);
		RuinMeshCommon->SetVisibility(true);
		break;
    	
	case EResourceType::UNCOMMON:
		ExtractionRingMesh->SetMaterial(0, UncommonMaterial);
		RuinMeshUncommon->SetVisibility(true);
		break;
    	
	case EResourceType::RARE:
		ExtractionRingMesh->SetMaterial(0, RareMaterial);
		RuinMeshRare->SetVisibility(true);
		break;
	}

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

		//old blueprint code that is probably dead
		UpdateBubble();
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

//sets mesh to nothing if there are no resources
void ARuin::UpdateBubble()
{
	if(ResourceAmount <= 0)
	{
		ExtractionRingMesh->SetVisibility(false);
	}
}

int32 ARuin::GetResourceAmount() const
{
	return ResourceAmount;	
}

//lowers the resource count by one
void ARuin::TakeOneResource()
{
	//lowers resource by one
	ResourceAmount--;

	//runs a check to see if we have resources left
	UpdateBubble();
}

//returns the type of resource this ruin has
EResourceType ARuin::GetResourceType() const
{
	return ResourceType;
}


int32 ARuin::GetInitialResourceAmount() const
{
	return StartingResourceAmount;
}
