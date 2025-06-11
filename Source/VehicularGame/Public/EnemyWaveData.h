#pragma once

#include "CoreMinimal.h"
#include "EnemyWaveData.generated.h"

class AEnemyCharacter;

USTRUCT(BlueprintType)
struct FEnemyTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Type")
	TSubclassOf<AEnemyCharacter> EnemyClass; // The enemy blueprint to spawn.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Type")
	int32 DifficultyCost; // "Cost" to spawn this enemy type. 
};