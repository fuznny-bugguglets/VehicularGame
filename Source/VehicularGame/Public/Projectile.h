#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h" // <--- ADD THIS INCLUDE
#include "Projectile.generated.h"

// Forward declarations
class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem; // Still useful for ExplosionEffect, EnemyHitParticles, WallHitParticles if they are Cascade
// class UParticleSystemComponent; // No longer needed if TrailPSC is Niagara
class UMaterialInstanceDynamic;
// class UNiagaraSystem; // Forward declare if you assign Niagara Systems directly in C++

UCLASS()
class VEHICULARGAME_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    AProjectile();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // --- COMPONENTS ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BulletMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* TrailPSC;

    // --- MOVEMENT PROPERTIES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
    float InitialSpeed = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
    float SphereCastRadius = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
    float GravityMultiplier = 0.0f;

    // --- COLLISION PROPERTIES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Collision")
    TArray<FName> IgnoredTags;

    // --- EXPLOSION PROPERTIES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Explosion")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Explosion")
    TEnumAsByte<ECollisionChannel> EnemyCollisionChannel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Explosion")
    float ExplosionDamage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Explosion")
    float ExplosionRange = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Explosion")
    float ExplosionForce = 500.0f;

    // --- LIFETIME PROPERTIES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Lifetime")
    int MaxCollisionsBeforeExplode = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Lifetime")
    float MaxLifetime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Lifetime")
    bool bExplodeOnTouch = true;

    // --- RENDERING & EFFECTS PROPERTIES ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Effects")
    UParticleSystem* EnemyHitParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Effects")
    UParticleSystem* WallHitParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Effects")
    float TrailFadeOutSpeed = 5.0f;

private:
    FVector CurrentVelocity;
    int CollisionCount = 0;
    bool bHasExploded = false;
    float TimeSinceExploded = 0.0f;
    float InitialTrailStartAlpha = 1.0f;
    float InitialTrailEndAlpha = 1.0f;
    UPROPERTY() // UPROPERTY to prevent garbage collection if assigned
    UMaterialInstanceDynamic* TrailMaterialDynamic = nullptr;

    // Helper functions
    void InitializeMovement();
    void UpdateMovement(float DeltaTime);
    bool PerformSweepCheck(const FVector& StartLocation, const FVector& EndLocation, float SweepRadius, FHitResult& OutHit);
    bool ShouldIgnoreHit(const AActor* HitActor, const UPrimitiveComponent* HitComponent) const;
    void ProcessHit(const FHitResult& Hit);
    void PlayHitEffect(const FVector& ImpactPoint, const FVector& ImpactNormal, bool bIsEnemy);
    void Explode();
    void UpdateTrailFade(float DeltaTime);

public:
#if WITH_EDITOR
    // For debug drawing in editor during play
    void DrawDebugShapes() const;
#endif
};