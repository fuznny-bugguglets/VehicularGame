// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stalker.generated.h"

class USplineComponent;
class AVehicle;

UENUM()
enum class EStalkerState : uint8
{
	WALKING,
	CHARGING,
	BUBBLE_GROWING,
	BUBBLE_GROWN
};

UCLASS()
class VEHICULARGAME_API AStalker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStalker();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	EStalkerState GetStalkerState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* StalkerMesh;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* DamageBallMesh;

private:
	UFUNCTION()
	void TickWalking(float DeltaTime);

	UFUNCTION()
	void TickCharging(float DeltaTime);

	UFUNCTION()
	void TickBubbleGrowing(float DeltaTime);

	UFUNCTION()
	void TickBubbleGrown(float DeltaTime);

	UFUNCTION()
	void TickOverlap(float DeltaTime);

	UFUNCTION()
	void OnBubbleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnBubbleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//called whenever the state should change
	void SetStalkerState(EStalkerState NewStalkerState);

	//reference to the spline to follow
	UPROPERTY(EditInstanceOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	AActor* SplineActorReference = nullptr;

	//how many seconds it takes to loop around the spline
	UPROPERTY(EditDefaultsOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	float TimeToFinishLoop = 10.0f;
	
	//how close the player has to be to start charging
	UPROPERTY(EditDefaultsOnly, Category="Attack", meta=(AllowPrivateAccess="true"))
	float AttackTriggerRange = 5000;
	
	//how big the attack radius will be
	UPROPERTY(EditDefaultsOnly, Category="Attack|Bubble", meta=(AllowPrivateAccess="true"))
	float AttackBubbleRadius = 15000;

	//how many seconds it takes to charge, before the bubble begins to grow
	UPROPERTY(EditDefaultsOnly, Category="Attack|Bubble", meta=(AllowPrivateAccess="true"))
	float AttackBubbleTimeToCharge = 2.0f;

	//how many seconds it takes to grow to its max attack range
	UPROPERTY(EditDefaultsOnly, Category="Attack|Bubble", meta=(AllowPrivateAccess="true"))
	float AttackBubbleGrowSpeed = 3.0f;

	//how many seconds the stalker will hold the fully grown bubble for
	UPROPERTY(EditDefaultsOnly, Category="Attack|Bubble", meta=(AllowPrivateAccess="true"))
	float AttackBubbleHoldTime = 5.0f;

	//on each damage event, the amount of damage applied to the player
	UPROPERTY(EditDefaultsOnly, Category="Attack|Damage", meta=(AllowPrivateAccess="true"))
	int32 DamageGiven = 5;

	//how many seconds for each damage hit
	UPROPERTY(EditDefaultsOnly, Category="Attack|Damage", meta=(AllowPrivateAccess="true"))
	float TimeBetweenDamageEvents = 0.5f;
	
	//the current AI state of this stalker
	UPROPERTY()
	EStalkerState StalkerState = EStalkerState::WALKING;

	//reference to the player
	UPROPERTY()
	AVehicle* Vehicle = nullptr;

	//reference to the spline component
	UPROPERTY()
	USplineComponent* SplineComponent = nullptr;

	//used for behaviour timers
	UPROPERTY()
	float ElapsedTime = 0.0f;

	//used for overlapping timer
	float ElapsedOverlapTime = 0.0f;

	//used for movement timer
	UPROPERTY()
	float MovementAlpha = 0.0f;

	//stores whether the player is in the damage bubble
	UPROPERTY()
	bool bIsOverlapping = false;

};
