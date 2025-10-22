// Fill out your copyright notice in the Description page of Project Settings.


#include "Ruin.h"
#include "VehicularGameInstance.h"
#include "VehicularGameMode.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void ARuin::LogError(const FString& ErrorMessage) const
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
	RuinEnteranceLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Ruin Enterance Location"));

	//setup attachments
	RuinMeshCommon->SetupAttachment(RootComponent);
	RuinMeshUncommon->SetupAttachment(RuinMeshCommon);
	RuinMeshRare->SetupAttachment(RuinMeshCommon);
	ExtractionRingMesh->SetupAttachment(RuinMeshCommon);
	SphereCollider->SetupAttachment(RuinMeshCommon);
	RuinEnteranceLocation->SetupAttachment(RuinMeshCommon);

	
	
	//disable collision on all meshes
	RuinMeshCommon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RuinMeshUncommon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RuinMeshRare->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	

	
}

void ARuin::FillUpRestOfType(int thisRandNumber)
{
	int32 probability = FMath::RandRange(0, 23);

	if (probability <= 10)
	{
		Resources.Add(0 + (thisRandNumber * 3));
	}
	else if (probability <= 18)
	{
		Resources.Add(1 + (thisRandNumber * 3));
	}
	else
	{
		Resources.Add(2 + (thisRandNumber * 3));
	}
}

// Called when the game starts or when spawned
void ARuin::BeginPlay()
{
	Super::BeginPlay();

	//hide all meshes
	RuinMeshCommon->SetVisibility(false);
	RuinMeshUncommon->SetVisibility(false);
	RuinMeshRare->SetVisibility(false);

	/*
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
	*/
	//temp
	ExtractionRingMesh->SetMaterial(0, CommonMaterial);
	RuinMeshCommon->SetVisibility(true);

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
	
	//set the resource amount to the default
	ResourceAmount = FMath::RandRange(StartingResourceAmountMin, StartingResourceAmountMax);
	InitialResourceAmount = ResourceAmount;

	//old blueprint code that is probably dead
	UpdateBubble();

	//fill us up with resources
	Resources.Empty();

	//generate a list with 0-4
	TArray<int32> tempNumbers;
	for (int32 i = 0; i < 5; i++)
	{
		tempNumbers.Add(i);
	}

	TArray<int32> selectedNumbers;

	int32 randomTypeProbability = FMath::RandRange(0, 53);

	switch (RuinType)
	{
	case ERuinType::Tier1:
		//pick 3 random numbers from that list (acts as relic types ABCDE)
			
		for (int32 i = 0; i < 3; i++)
		{
			int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
			int32 randNumber = tempNumbers[randIndex];
			tempNumbers.Remove(randNumber);
			selectedNumbers.Add(randNumber);
		}

		//fill up the resources
		for (int32 i = 0; i < ResourceAmount; i++)
		{
			int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
			int32 thisRandNumber = selectedNumbers[thisRandIndex];
			int32 probability = FMath::RandRange(0, 33);

			if (probability <= 25)
			{
				Resources.Add(0 + (thisRandNumber * 3));
			}
			else if (probability <= 32)
			{
				Resources.Add(1 + (thisRandNumber * 3));
			}
			else
			{
				Resources.Add(2 + (thisRandNumber * 3));
			}
		}
		
		break;

		case ERuinType::Tier2:
			//pick 3 random numbers from that list (acts as relic types ABCDE)
			for (int32 i = 0; i < 3; i++)
			{
				int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
				int32 randNumber = tempNumbers[randIndex];
				tempNumbers.Remove(randNumber);
				selectedNumbers.Add(randNumber);
			}

			//fill up the resources
			for (int32 i = 0; i < ResourceAmount; i++)
			{
				int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
				int32 thisRandNumber = selectedNumbers[thisRandIndex];
				int32 probability = FMath::RandRange(0, 33);

				if (probability <= 10)
				{
					Resources.Add(0 + (thisRandNumber * 3));
				}
				else if (probability <= 30)
				{
					Resources.Add(1 + (thisRandNumber * 3));
				}
				else
				{
					Resources.Add(2 + (thisRandNumber * 3));
				}
			}
			break;

		case ERuinType::TypeA:
			//add a random tiered type A relic
			
			if (randomTypeProbability <= 15)
			{
				Resources.Add(0);
			}
			else if (randomTypeProbability <= 33)
			{
				Resources.Add(1);
			}
			else
			{
				Resources.Add(2);
			}

			//remove A from the pool
			tempNumbers.Remove(0);

			//add it to our selected numbers
			selectedNumbers.Add(0);

			//populate it with 2 more
			for (int32 i = 0; i < 2; i++)
			{
				int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
				int32 randNumber = tempNumbers[randIndex];
				tempNumbers.Remove(randNumber);
				selectedNumbers.Add(randNumber);
			}

			//fill up the resources
			for (int32 i = 0; i < ResourceAmount - 1; i++)
			{
				int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
				int32 thisRandNumber = selectedNumbers[thisRandIndex];
				

				//is this 'A'
				if (thisRandNumber == 0)
				{
					int32 probability = FMath::RandRange(0, 53);

					if (probability <= 15)
					{
						Resources.Add(0);
					}
					else if (probability <= 33)
					{
						Resources.Add(1);
					}
					else
					{
						Resources.Add(2);
					}
				}

				//otherwise
				else
				{
					FillUpRestOfType(thisRandNumber);
				}
			}
			break;

		case ERuinType::TypeB:
			//add a random tiered type A relic
			
			if (randomTypeProbability <= 15)
			{
				Resources.Add(3);
			}
			else if (randomTypeProbability <= 33)
			{
				Resources.Add(4);
			}
			else
			{
				Resources.Add(5);
			}

			//remove B from the pool
			tempNumbers.Remove(1);

			//add it to our selected numbers
			selectedNumbers.Add(1);

			//populate it with 2 more
			for (int32 i = 0; i < 2; i++)
			{
				int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
				int32 randNumber = tempNumbers[randIndex];
				tempNumbers.Remove(randNumber);
				selectedNumbers.Add(randNumber);
			}

			//fill up the resources
			for (int32 i = 0; i < ResourceAmount - 1; i++)
			{
				int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
				int32 thisRandNumber = selectedNumbers[thisRandIndex];


				//is this 'B'
				if (thisRandNumber == 1)
				{
					int32 probability = FMath::RandRange(0, 53);

					if (probability <= 15)
					{
						Resources.Add(0);
					}
					else if (probability <= 33)
					{
						Resources.Add(1);
					}
					else
					{
						Resources.Add(2);
					}
				}

				//otherwise
				else
				{
					FillUpRestOfType(thisRandNumber);
				}
			}
			break;

		case ERuinType::TypeC:
			//add a random C type relic
			if (randomTypeProbability <= 15)
			{
				Resources.Add(6);
			}
			else if (randomTypeProbability <= 33)
			{
				Resources.Add(7);
			}
			else
			{
				Resources.Add(8);
			}

			//remove C from the pool
			tempNumbers.Remove(2);

			//add it to our selected numbers
			selectedNumbers.Add(2);

			//populate it with 2 more
			for (int32 i = 0; i < 2; i++)
			{
				int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
				int32 randNumber = tempNumbers[randIndex];
				tempNumbers.Remove(randNumber);
				selectedNumbers.Add(randNumber);
			}

			//fill up the resources
			for (int32 i = 0; i < ResourceAmount - 1; i++)
			{
				int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
				int32 thisRandNumber = selectedNumbers[thisRandIndex];


				//is this 'C'
				if (thisRandNumber == 2)
				{
					int32 probability = FMath::RandRange(0, 53);

					if (probability <= 15)
					{
						Resources.Add(0);
					}
					else if (probability <= 33)
					{
						Resources.Add(1);
					}
					else
					{
						Resources.Add(2);
					}
				}

				//otherwise
				else
				{
					FillUpRestOfType(thisRandNumber);
				}
			}
			break;

		case ERuinType::TypeD:
			//add a random tiered type D relic

			if (randomTypeProbability <= 15)
			{
				Resources.Add(9);
			}
			else if (randomTypeProbability <= 33)
			{
				Resources.Add(10);
			}
			else
			{
				Resources.Add(11);
			}

			//remove D from the pool
			tempNumbers.Remove(3);

			//add it to our selected numbers
			selectedNumbers.Add(3);

			//populate it with 2 more
			for (int32 i = 0; i < 2; i++)
			{
				int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
				int32 randNumber = tempNumbers[randIndex];
				tempNumbers.Remove(randNumber);
				selectedNumbers.Add(randNumber);
			}

			//fill up the resources
			for (int32 i = 0; i < ResourceAmount - 1; i++)
			{
				int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
				int32 thisRandNumber = selectedNumbers[thisRandIndex];


				//is this 'D'
				if (thisRandNumber == 3)
				{
					int32 probability = FMath::RandRange(0, 53);

					if (probability <= 15)
					{
						Resources.Add(0);
					}
					else if (probability <= 33)
					{
						Resources.Add(1);
					}
					else
					{
						Resources.Add(2);
					}
				}

				//otherwise
				else
				{
					FillUpRestOfType(thisRandNumber);
				}
			}
			break;

		case ERuinType::TypeE:
			//add a random tiered type E relic

			if (randomTypeProbability <= 15)
			{
				Resources.Add(12);
			}
			else if (randomTypeProbability <= 33)
			{
				Resources.Add(13);
			}
			else
			{
				Resources.Add(14);
			}

			//remove E from the pool
			tempNumbers.Remove(4);

			//add it to our selected numbers
			selectedNumbers.Add(4);

			//populate it with 2 more
			for (int32 i = 0; i < 2; i++)
			{
				int32 randIndex = FMath::RandRange(0, tempNumbers.Num() - 1);
				int32 randNumber = tempNumbers[randIndex];
				tempNumbers.Remove(randNumber);
				selectedNumbers.Add(randNumber);
			}

			//fill up the resources
			for (int32 i = 0; i < ResourceAmount - 1; i++)
			{
				int32 thisRandIndex = FMath::RandRange(0, selectedNumbers.Num() - 1);
				int32 thisRandNumber = selectedNumbers[thisRandIndex];


				//is this 'E'
				if (thisRandNumber == 4)
				{
					int32 probability = FMath::RandRange(0, 53);

					if (probability <= 15)
					{
						Resources.Add(0);
					}
					else if (probability <= 33)
					{
						Resources.Add(1);
					}
					else
					{
						Resources.Add(2);
					}
				}

				//otherwise
				else
				{
					FillUpRestOfType(thisRandNumber);
				}
			}
			break;

		default:
			break;
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
uint32 ARuin::TakeOneResource()
{
	//lowers resource by one
	ResourceAmount--;

	//grab the game mode
	AVehicularGameMode* VGameMode = Cast<AVehicularGameMode>(UGameplayStatics::GetGameMode(this));
	if (!VGameMode)
	{
		LogError("FAILED TO GET VEHICULAR GAME MODE IN RUIN");
		return 0;
	}

	//tell the game mode we extracted (for UI)
	VGameMode->RuinExtractionCountUpdated();

	//runs a check to see if we have resources left
	UpdateBubble();

	//returns the type of resource taken
	return Resources[ResourceAmount];
}

//returns the type of resource this ruin has
EResourceType ARuin::GetResourceType() const
{
	return EResourceType::NULLRESOURCE;
}


int32 ARuin::GetInitialResourceAmount() const
{
	return InitialResourceAmount;
}

FVector ARuin::GetEnteranceLocation() const
{
	if(!RuinEnteranceLocation)
	{
		LogError("failed to access ruin entrance location");
		return FVector(0.0f);
	}
	return RuinEnteranceLocation->GetComponentLocation();
}

float ARuin::GetExtractionTime() const
{
	return ExtractionTimePerRelic;
}
