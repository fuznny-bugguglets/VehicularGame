#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CurrentBounces = 0;
    MaxBounces = 1;

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
    // ProjectileMovement->ImpactForce = 0.0f; // << REMOVE THIS LINE
    ProjectileMovement->Bounciness = 0.6f;

    TrailNSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNSC"));
    TrailNSC->SetupAttachment(RootComponent);

    InitialLifeSpan = 3.0f;

    ImpactForceMagnitude = 100.0f; // Default value for the force applied to other objects
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

    if (OtherComp->IsSimulatingPhysics())
    {
        // Use the ImpactForceMagnitude variable here
        OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity.GetSafeNormal() * ImpactForceMagnitude, Hit.ImpactPoint);
    }

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
    else
    {
        // Bounce logic handled by ProjectileMovementComponent
    }
}