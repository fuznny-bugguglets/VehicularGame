// Fill out your copyright notice in the Description page of Project Settings.


#include "ScavengerPawn.h"
#include "AIController.h"
#include "Vehicle.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" // For character movement

void AScavengerPawn::LogError(const FString& ErrorMessage)
{
	//if we have the engine pointer, we print to the screen
	if (GEngine)
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
AScavengerPawn::AScavengerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScavengerPawn::BeginPlay()
{
	Super::BeginPlay(); 

	//get a reference to the ai controller
	AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		LogError("Failed to get AI Controller in Scavenger");
	}

	//by default, go to the ruin
	CurrentBehaviour = EScavengerBehaviourState::TRAVELLING_TO_RUIN;


}

// Called every frame
void AScavengerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//switch based on current behaviour
	switch (CurrentBehaviour)
	{
	case EScavengerBehaviourState::TRAVELLING_TO_RUIN:
		DoTravelToRuin();
		break;
	
	case EScavengerBehaviourState::SCAVENGING:
		DoScavenge(DeltaTime);
		break;

	case EScavengerBehaviourState::TRAVELLING_TO_TRUCK:
		DoTravelToTruck();
	}

}

void AScavengerPawn::DoTravelToRuin()
{
	//is there a ruin?
	if (!MyRuin)
	{
		LogError("scavenger is set to travel to a ruin but doesn't have a ruin to travel to. it will now idle");
		//CurrentBehaviour = EScavengerBehaviourState::IDLE;
		return;
	}

	//are we close enough to the ruin?
	if (FVector::DistSquared(GetActorLocation(), MyRuin->GetEnteranceLocation()) <= TargetTolerance*TargetTolerance)
	{
		LogError("arrived at ruin entrance location");
		ElapsedScavengeTime = 0.0f;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		CurrentBehaviour = EScavengerBehaviourState::SCAVENGING;
	}
	
}

void AScavengerPawn::DoScavenge(float DeltaTime)
{
	//is there a ruin?
	if (!MyRuin)
	{
		LogError("scavenger is set to scavenge but doesn't have a ruin to travel to. it will now idle");
		CurrentBehaviour = EScavengerBehaviourState::IDLE;
		return;
	}

	//does the ruin have resources?
	if (MyRuin->GetResourceAmount() <= 0)
	{
		LogError("no more resources, returning to truck");
		GoToTruck();
		return;
	}

	//do your time
	ElapsedScavengeTime += DeltaTime;

	//have we done our time?
	if (ElapsedScavengeTime >= MyRuin->GetExtractionTime())
	{
		//take a resource
		HeldResource = MyRuin->TakeOneResource();

		//go back to the truck
		GoToTruck();
	}


}

void AScavengerPawn::DoTravelToTruck()
{
	//is there a truck?
	if (!MyVehicle)
	{
		LogError("scavenger is set to travel to a truck but doesn't have a truck to travel to. it will now idle");
		//CurrentBehaviour = EScavengerBehaviourState::IDLE;
		return;
	}

	//update target position
	GoToTruck();

	//are we close enough to the truck door?
	if (FVector::DistSquared(GetActorLocation(), MyVehicle->GetDoorLocation()) <= TargetTolerance * TargetTolerance)
	{
		LogError("arrived at door entrance location");

		//give the resource to the truck
		MyVehicle->IncrementLootCount(HeldResource);
		HeldResource = 9999;


		//is there a ruin?
		if (MyRuin)
		{
			//are there more resources to grab?
			if (MyRuin->GetResourceAmount() > 0)
			{
				GoToRuin();
				return;
			}
		}

		MyVehicle->ReturnScavenger(this);

	}
}




// Called to bind functionality to input
void AScavengerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AScavengerPawn::MoveTo(const FVector& TargetLocation)
{
	if (!AIController)
	{
		LogError("Failed to get AI Controller in Scavenger");
		return;
	}

	AIController->MoveToLocation(TargetLocation);
}

void AScavengerPawn::SetLocations(ARuin* InputRuin, AVehicle* InputVehicle)
{
	SetRuin(InputRuin);
	SetVehicle(InputVehicle);
}

void AScavengerPawn::SetRuin(ARuin* InputRuin)
{
	MyRuin = InputRuin;
}

void AScavengerPawn::SetVehicle(AVehicle* InputVehicle)
{
	MyVehicle = InputVehicle;
}

void AScavengerPawn::GoToRuin()
{
	CurrentBehaviour = EScavengerBehaviourState::TRAVELLING_TO_RUIN;

	if (!MyRuin)
	{
		LogError("cant travel to ruin because scavy doesnt have a ruin set");
		return;
	}

	if (!AIController)
	{
		LogError("Failed to get AI Controller in Scavenger");
		return;
	}

	AIController->MoveToLocation(MyRuin->GetEnteranceLocation());
}

void AScavengerPawn::GoToTruck()
{
	CurrentBehaviour = EScavengerBehaviourState::TRAVELLING_TO_TRUCK;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (!MyVehicle)
	{
		LogError("cant travel to truck because scavy doesnt have a truck set");
		return;
	}

	if (!AIController)
	{
		LogError("Failed to get AI Controller in Scavenger");
		return;
	}

	AIController->MoveToLocation(MyVehicle->GetDoorLocation());
}
