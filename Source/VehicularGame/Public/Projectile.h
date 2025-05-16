#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent; // For Niagara
class UNiagaraSystem;    // For Niagara System Asset

UCLASS(config = Game)
class AProjectile : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* TrailNSC;

public:
    AProjectile();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    USphereComponent* GetCollisionComp() const { return CollisionComp; }
    UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects)
    UNiagaraSystem* TrailNiagaraSystemAsset;

private:
    UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin = "0")) // Ensure MaxBounces can be 0 for destroy on first hit
        int32 MaxBounces;

    int32 CurrentBounces;
};