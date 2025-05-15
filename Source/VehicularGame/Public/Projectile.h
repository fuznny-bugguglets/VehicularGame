#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent; // For Niagara
class UNiagaraSystem;    // For Niagara System Asset

UCLASS(config=Game)
class AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// Niagara System Component that will play the trail
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* TrailNSC; // NSC for Niagara System Component

public:
	AProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Niagara system asset to use for the trail - assignable in Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects)
	UNiagaraSystem* TrailNiagaraSystemAsset;

private:
	int32 CurrentBounces;
	UPROPERTY(EditAnywhere, Category = "Projectile")
	int32 MaxBounces;
};