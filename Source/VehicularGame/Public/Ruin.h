// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceTypes.h"
#include "Ruin.generated.h"

class USphereComponent;



UCLASS()
class VEHICULARGAME_API ARuin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuin();

	//returns the amount of resources the ruin has
	UFUNCTION(BlueprintCallable)
	int32 GetResourceAmount() const;

	UFUNCTION(BlueprintCallable)
	int32 GetInitialResourceAmount() const;

	//lowers the resource count by one
	EResourceType TakeOneResource();

	//returns the type of resource this ruin contains
	EResourceType GetResourceType() const;

	//returns the position of the enterance of the ruin
	FVector GetEnteranceLocation() const;

	//returns how long it takes to take a resource
	UFUNCTION(BlueprintCallable)
	float GetExtractionTime() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//sets mesh to nothing if there are no resources
	void UpdateBubble();
	
	//how much the ruin starts with
	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	int32 StartingResourceAmount = 1;

	//how much the ruin currently as
	int32 ResourceAmount = 0;

	//the type of resource this is
	UPROPERTY(EditInstanceOnly, Category = "Resource Type", meta = (AllowPrivateAccess = "true"))
	EResourceType ResourceType = EResourceType::A;

	//materials
	UPROPERTY(EditDefaultsOnly, Category = "Ring Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* CommonMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Ring Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* UncommonMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Ring Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* RareMaterial;
	
	//mesh types
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RuinMeshCommon;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RuinMeshUncommon;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* RuinMeshRare;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ExtractionRingMesh;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereCollider = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RuinEnteranceLocation = nullptr;

private:
	void LogError(const FString& ErrorMessage) const;


	//how long extractions take
	UPROPERTY(EditAnywhere, Category = "Extractions", meta = (AllowPrivateAccess = "true"))
	float ExtractionTimePerRelic = 8.0f;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
