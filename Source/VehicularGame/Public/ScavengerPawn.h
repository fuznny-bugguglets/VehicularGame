// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ScavengerPawn.generated.h"


class AVehicle;
class AAIController;
class ARuin;

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
	
	UPROPERTY()
	AAIController* AIController = nullptr;

	UPROPERTY()
	ARuin* MyRuin;

	UPROPERTY()
	AVehicle* MyVehicle;

	FVector MyTarget = FVector(0.0f);

	void LogError(const FString& ErrorMessage);

};
