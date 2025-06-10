#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UStaticMeshComponent; // Forward declaration

UCLASS(config = Game)
class AProjectile : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComp;

    // We no longer create this in C++, so the UPROPERTY macro is removed.
    // This will be a pointer to the Static Mesh Component that already exists on the Blueprint.
    UStaticMeshComponent* ProjectileMesh; 

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0.0"))
    float ImpactForceMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0.0"))
    float Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0.0"))
    float LingerDuration;

private:
    UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin = "0"))
    int32 MaxBounces;

    int32 CurrentBounces;
    
    bool bIsDying;
};