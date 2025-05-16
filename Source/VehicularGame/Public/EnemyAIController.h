// EnemyAIController.h
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

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override; // Called when the controller possesses a pawn

public:
	virtual void Tick(float DeltaTime) override; // Called every frame

private:
	UPROPERTY()
	class UBehaviorTreeComponent* BehaviorTreeComponent; // Component to run a behavior tree

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent; // Component to store AI data

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetPlayerKeyName; // Blackboard key name for the target player

	void SetTargetPlayer(APawn* PlayerPawn); // Sets the player as target in blackboard
};