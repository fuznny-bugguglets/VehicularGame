#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class VEHICULARGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "AI|Targeting")
	FVector GetMovingTargetLocation() const;

protected:
	// --- TARGETING PARAMETERS ---
	// How far in front of the player should the targeting line be 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (ClampMin = "0.0"))
	float TargetLineForwardOffset;

	// How wide should the targeting line span on either side of the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (ClampMin = "0.0"))
	float TargetLineHalfWidth;

	// How fast should the target point move along the target line?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (ClampMin = "0.0"))
	float TargetPointNoiseSpeed;

	// --- GROUND PROJECTION PARAMETERS ---
	// How high above the calculated point to start the ground trace.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (ClampMin = "0.0"))
	float GroundTraceUpwardOffset;

	// How far down to trace for the ground from the starting point.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Targeting", meta = (ClampMin = "0.0"))
	float GroundTraceDownwardDistance;

private:
	float NoiseTimeAccumulator;
	FVector CurrentTargetLocation;
};