// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ruin.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	COMMON UMETA(DisplayName = "Common"),
	UNCOMMON UMETA(DisplayName = "Uncommon"),
	RARE UMETA(DisplayName = "Rare")
};

UCLASS()
class VEHICULARGAME_API ARuin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuin();

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
	EResourceType ResourceType;

	//materials
	UPROPERTY(EditDefaultsOnly, Category = "Ring Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* CommonMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Ring Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* UncommonMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Ring Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* RareMaterial;
	
	//mesh types
	UPROPERTY()
	UStaticMeshComponent* RuinMeshCommon;
	UPROPERTY()
	UStaticMeshComponent* RuinMeshUncommon;
	UPROPERTY()
	UStaticMeshComponent* RuinMeshRare;
	UPROPERTY()
	UStaticMeshComponent* ExtractionRingMesh;

private:
	void LogError(const FString& ErrorMessage);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
