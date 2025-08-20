// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ruin.h"
#include "GameFramework/Character.h"
#include "ScavengerPawn.generated.h"


class AVehicle;
class AAIController;

UENUM()
enum class EScavengerBehaviourState : uint8
{
	SCAVENGING UMETA(DisplayName = "SCAVENGING"),
	TRAVELLING_TO_TRUCK UMETA(DisplayName = "Travelling To Truck"),
	TRAVELLING_TO_RUIN UMETA(DisplayName = "Travelling To Ruin"),
	IDLE UMETA(DisplayName = "Idle")
};

UCLASS()

class VEHICULARGAME_API AScavengerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AScavengerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GoToRuin();
	void GoToTruck();

	void SetLocations(ARuin* InputRuin, AVehicle* InputVehicle);
	void SetRuin(ARuin* InputRuin);
	void SetVehicle(AVehicle* InputVehicle);

	void MoveTo(const FVector& TargetLocation);

private:

	// how close (in cm) i need to be to a target before i am marked as arrived
	UPROPERTY(EditDefaultsOnly, Category = "Pathfinding", meta = (AllowPrivateAccess = "true"))
	float TargetTolerance = 100.0f;

	UPROPERTY()
	AAIController* AIController = nullptr;

	UPROPERTY()
	ARuin* MyRuin;

	UPROPERTY()
	AVehicle* MyVehicle;

	//location of where i should move to 
	FVector MyTarget = FVector(0.0f);

	//what kind of behaviour i should do 
	EScavengerBehaviourState CurrentBehaviour = EScavengerBehaviourState::IDLE;

	//what resource i am holdingh
	EResourceType HeldResource = EResourceType::NULLRESOURCE;

	//time i have been scavenging
	float ElapsedScavengeTime = 0.0f;

	//called every frame when my behaviour is travelling to the ruin
	void DoTravelToRuin();

	//called every frame while travelling to the truck
	void DoTravelToTruck();

	//called every frame when my behaviour is set to scavenging
	void DoScavenge(float DeltaTime);

	void LogError(const FString& ErrorMessage);

};
