#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class VEHICULARGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter(); // Constructor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MaxHealth; // Maximum health of the enemy

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	float CurrentHealth; // Current health of the enemy

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; // Handles damage taken

protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned

public:
	virtual void Tick(float DeltaTime) override; // Called every frame

	void Die(); // Handles death logic

	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool IsDead() const; // Checks if the enemy is dead
};