#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UUpgradeSubsystem;
class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UStaticMeshComponent;

// Declares the new event dispatcher that will be broadcast on hit.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileImpact, const FVector&, ImpactLocation, AActor*, HitActor);

UCLASS(config = Game)
class VEHICULARGAME_API AProjectile : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
    USphereComponent* CollisionComp;

    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* TrailNSC;

public:
    AProjectile();

    // This new function will set the projectile's stats after it has been spawned.
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void InitializeProjectile(float InAdditionalDamage, int32 InPiercingCount, float InSpeedMultiplier);

    // This is the event dispatcher that Blueprints can bind to.
    UPROPERTY(BlueprintAssignable, Category = "Projectile")
    FOnProjectileImpact OnProjectileImpact;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // Getters
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

    // The new variable to track how many enemies this projectile can pierce.
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (ClampMin = "0"))
    int32 ProjectilePiercing;

private:
    UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin = "0"))
    int32 MaxBounces;

    int32 CurrentBounces;
    bool bIsDying;

    //pointer to the upgrade subsystem
    UPROPERTY()
    UUpgradeSubsystem* UpgradeSubsystem = nullptr;

    //returns the upgrade subsystem
    UFUNCTION()
    UUpgradeSubsystem* GetUpgradeSubsystem();

};