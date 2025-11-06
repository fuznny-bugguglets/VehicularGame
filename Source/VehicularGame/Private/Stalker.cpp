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

	USceneComponent* TempSceneComponent = CreateDefaultSubobject<USceneComponent>("Scene Comp");
	SetRootComponent(TempSceneComponent);
	
	StalkerMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Stalker Mesh");
	StalkerMesh->SetupAttachment(TempSceneComponent);

	DamageBallMesh = CreateDefaultSubobject<UStaticMeshComponent>("Damage Ball Mesh");
	DamageBallMesh->SetupAttachment(StalkerMesh);

}

// Called when the game starts or when spawned
void AStalker::BeginPlay()
{
	Super::BeginPlay();

	//find the player
	Vehicle = Cast<AVehicle>(UGameplayStatics::GetActorOfClass(this, AVehicle::StaticClass()));
	checkf(Vehicle, TEXT("Tried to get a reference to the player stalker, but couldn't find anything"));

	//get the spline
	checkf(SplineActorReference, TEXT("No spline actor reference for %s"), *GetActorNameOrLabel());
	SplineComponent = SplineActorReference->GetComponentByClass<USplineComponent>();
	checkf(SplineComponent, TEXT("Failed to find a spline component in %s, which is referenced in"),
		*SplineActorReference->GetActorNameOrLabel(), *GetActorNameOrLabel());

	//ball
	DamageBallMesh->SetRelativeScale3D(FVector(0.01f));
	DamageBallMesh->OnComponentBeginOverlap.AddDynamic(this, &AStalker::OnBubbleOverlap);
	DamageBallMesh->OnComponentEndOverlap.AddDynamic(this, &AStalker::OnBubbleEndOverlap);
	
	
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
	if (MovementAlpha > 1.0f)
	{
		MovementAlpha -= 1.0f;
	}
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

	//set bubble size
	const float BubbleScale = (ElapsedTime/AttackBubbleGrowSpeed) * AttackBubbleRadius;
	DamageBallMesh->SetRelativeScale3D(FVector(BubbleScale));

	//has it been enough time yet?
	if (ElapsedTime >= AttackBubbleGrowSpeed)
	{
		//end growing
		SetStalkerState(BUBBLE_GROWN);
	}

	TickOverlap(DeltaTime);
}

void AStalker::TickBubbleGrown(float DeltaTime)
{
	//increment time
	ElapsedTime += DeltaTime;

	//has it been enough time yet?
	if (ElapsedTime >= AttackBubbleHoldTime)
	{
		DamageBallMesh->SetRelativeScale3D(FVector(0.01f));
		SetStalkerState(WALKING);
	}

	TickOverlap(DeltaTime);
}

void AStalker::TickOverlap(float DeltaTime)
{
	//is the player in the damage bubble?
	if (bIsOverlapping)
	{
		//increment time in the bubble
		ElapsedOverlapTime += DeltaTime;

		//has the player spent too long in the bubble?
		if (ElapsedOverlapTime >= TimeBetweenDamageEvents)
		{
			//damage them
			ElapsedOverlapTime = 0.0f;
			const TSubclassOf<UDamageType> DamageType;
			UGameplayStatics::ApplyDamage(Vehicle, DamageGiven, nullptr, this, DamageType);
			
		}
	}
}


void AStalker::OnBubbleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//it should only ever damage with vehicle
	bIsOverlapping = true;
}

void AStalker::OnBubbleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapping = false;
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
