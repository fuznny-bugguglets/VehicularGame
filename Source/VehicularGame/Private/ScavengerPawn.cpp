// Fill out your copyright notice in the Description page of Project Settings.


#include "ScavengerPawn.h"
#include "AIController.h"
#include "Ruin.h"
#include "Vehicle.h"
#include "GameFramework/CharacterMovementComponent.h" // For character movement

void AScavengerPawn::LogError(const FString& ErrorMessage)
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
	if(!AIController)
	{
		LogError("Failed to get AI Controller in Scavenger");
	}

	
}

// Called every frame
void AScavengerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AScavengerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AScavengerPawn::MoveTo(const FVector& TargetLocation)
{
	if(!AIController)
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



