#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
// It's good practice to include any actor types you specifically want to damage for casting,
// for example, your enemy base class. Let's assume you have an "EnemyCharacter.h"
// #include "EnemyCharacter.h" // Example: If your enemy class is AEnemyCharacter

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CurrentBounces = 0;
    MaxBounces = 1;
    Damage = 10.0f; // Default damage value
    ImpactForceMagnitude = 100.0f;

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;
    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.6f;

    TrailNSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNSC"));
    TrailNSC->SetupAttachment(RootComponent);

    InitialLifeSpan = 3.0f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (TrailNiagaraSystemAsset)
    {
        TrailNSC->SetAsset(TrailNiagaraSystemAsset);
        TrailNSC->Activate(true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AProjectile (%s) has no TrailNiagaraSystemAsset assigned for its TrailNSC."), *GetName());
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this || !OtherComp || OtherActor == GetInstigator())
    {
        return;
    }

    // Apply physics impulse if the other component simulates physics
    if (OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity.GetSafeNormal() * ImpactForceMagnitude, Hit.ImpactPoint);
    }

    // --- DAMAGE APPLICATION LOGIC ---
    if (Damage > 0.f && OtherActor)
    {
        // The projectile's Instigator is the one who fired it.
        AController* EventInstigator = GetInstigatorController();
        AActor* DamageCauser = this;

        // Create a damage event
        FDamageEvent DamageEvent; // You can use FPointDamageEvent for more detail if needed
        // FPointDamageEvent PointDamageEvent;
        // PointDamageEvent.HitInfo = Hit; // If you want to pass the full hit result

        // Apply the damage
        OtherActor->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

        // Example: If you wanted to only damage AEnemyCharacter and have specific logic:
        // AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OtherActor);
        // if (Enemy)
        // {
        //     Enemy->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
        // }
    }
    // --- END DAMAGE APPLICATION LOGIC ---

    CurrentBounces++;
    bool bShouldBeDestroyed = false;

    if (!ProjectileMovement->bShouldBounce || (CurrentBounces >= MaxBounces))
    {
        bShouldBeDestroyed = true;
    }

    if (bShouldBeDestroyed)
    {
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Velocity = FVector::ZeroVector;
        SetActorEnableCollision(false);

        if (TrailNSC && TrailNSC->IsActive())
        {
            TrailNSC->Deactivate();
            TrailNSC->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }
        Destroy();
    }
    // else: Bounce logic is inherently handled by ProjectileMovementComponent if bShouldBounce is true
}