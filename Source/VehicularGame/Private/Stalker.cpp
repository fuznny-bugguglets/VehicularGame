// Fill out your copyright notice in the Description page of Project Settings.


#include "Stalker.h"

#include "Vehicle.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

using enum EStalkerState;

// Sets default values
AStalker::AStalker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStalker::BeginPlay()
{
	Super::BeginPlay();

	//find the player
	Vehicle = Cast<AVehicle>(UGameplayStatics::GetActorOfClass(this, AVehicle::StaticClass()));
	checkf(Vehicle, TEXT("Tried to get a reference to the player stalker, but couldn't find anything"));

	//get the children in the spline blueprint actor
	checkf(SplineActorReference, TEXT("No spline actor reference for %s"), *GetActorNameOrLabel());
	TArray<AActor*> SplineActorChildren;
	SplineActorReference->GetAllChildActors(SplineActorChildren);

	//find the spline component
	for (AActor* Child : SplineActorChildren)
	{
		if (USplineComponent* TempSplineComponent = Cast<USplineComponent>(Child))
		{
			SplineComponent = TempSplineComponent;
		}
	}
	checkf(SplineActorReference, TEXT("Failed to find a spline component in %s, which is referenced in"),
		*SplineActorReference->GetActorNameOrLabel(), *GetActorNameOrLabel());
	
}

// Called every frame
void AStalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	switch (StalkerState)
	{
	case WALKING:
		TickWalking(DeltaTime);
		break;
	case CHARGING:
		TickCharging(DeltaTime);
		break;
	case BUBBLE_GROWING:
		TickBubbleGrowing(DeltaTime);
		break;
	case BUBBLE_GROWN:
		TickBubbleGrown(DeltaTime);
		break;
	}

}

void AStalker::TickWalking(float DeltaTime)
{
	//increment along the spline
	MovementAlpha += DeltaTime / TimeToFinishLoop;
	const float LerpPoint = FMath::Lerp(0.0f, SplineComponent->GetSplineLength(), MovementAlpha);

	//get location and rotation at that point
	const FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LerpPoint, ESplineCoordinateSpace::World);
	const FRotator NewRotation = SplineComponent->GetRotationAtDistanceAlongSpline(LerpPoint, ESplineCoordinateSpace::World);

	//update actor position
	SetActorLocationAndRotation(NewLocation, NewRotation);
	
	//are we close enough to the player
	if (FVector::DistSquared(Vehicle->GetActorLocation(), GetActorLocation()) < AttackTriggerRange*AttackTriggerRange)
	{
		//start charging
		SetStalkerState(CHARGING);
	}
}

void AStalker::TickCharging(float DeltaTime)
{
	//increment time
	ElapsedTime += DeltaTime;

	//has it been enough time yet?
	if (ElapsedTime >= AttackBubbleTimeToCharge)
	{
		//begin growing
		SetStalkerState(BUBBLE_GROWING);
	}
}

void AStalker::TickBubbleGrowing(float DeltaTime)
{
	//increment time
	ElapsedTime += DeltaTime;

	//has it been enough time yet?
	if (ElapsedTime >= AttackBubbleGrowSpeed)
	{
		//end growing
		SetStalkerState(BUBBLE_GROWN);
	}
}

void AStalker::TickBubbleGrown(float DeltaTime)
{
	//increment time
	ElapsedTime += DeltaTime;

	//has it been enough time yet?
	if (ElapsedTime >= AttackBubbleHoldTime)
	{
		SetStalkerState(WALKING);
	}
}

void AStalker::SetStalkerState(EStalkerState NewStalkerState)
{
	//reset timer
	ElapsedTime = 0.0f;

	//set new state
	StalkerState = NewStalkerState;
}


EStalkerState AStalker::GetStalkerState() const
{
	return StalkerState;
}
