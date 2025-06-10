#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Initialize variables
    CurrentBounces = 0;
    MaxBounces = 1;
    Damage = 10.0f;
    ImpactForceMagnitude = 100.0f;
    LingerDuration = 3.0f;
    bIsDying = false;

    // Setup the root collision component
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    RootComponent = CollisionComp;

    // --- REMOVED MESH CREATION ---
    // The Static Mesh Component is expected to already exist on the Blueprint,
    // so we no longer create it here. We will find it in BeginPlay.
    ProjectileMesh = nullptr; // Initialize pointer as null

    // Setup the trail component and attach to root
    TrailNSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNSC"));
    TrailNSC->SetupAttachment(RootComponent);

    // Setup the projectile movement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.6f;

    InitialLifeSpan = 10.0f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    // --- FIND THE EXISTING STATIC MESH COMPONENT ---
    // This finds the first component of this type on the actor.
    // This allows you to add and manage the mesh in the Blueprint editor.
    ProjectileMesh = FindComponentByClass<UStaticMeshComponent>();
    if (!ProjectileMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("AProjectile (%s) could not find a UStaticMeshComponent."), *GetNameSafe(this));
    }

    // Activate the trail Niagara system if it's assigned
    if (TrailNiagaraSystemAsset)
    {
        TrailNSC->SetAsset(TrailNiagaraSystemAsset);
        TrailNSC->Activate(true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AProjectile (%s) has no TrailNiagaraSystemAsset assigned."), *GetNameSafe(this));
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bIsDying)
    {
        return;
    }

    if (!OtherActor || OtherActor == this || OtherActor == GetInstigator())
    {
        return;
    }

    // Apply impulse and damage
    if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity.GetSafeNormal() * ImpactForceMagnitude, Hit.ImpactPoint);
    }
    if (Damage > 0.f)
    {
        FDamageEvent DamageEvent;
        AController* EventInstigator = GetInstigatorController();
        OtherActor->TakeDamage(Damage, DamageEvent, EventInstigator, this);
    }

    CurrentBounces++;

    // Check if the projectile should be destroyed
    if (!ProjectileMovement->bShouldBounce || (CurrentBounces > MaxBounces))
    {
        bIsDying = true;

        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Velocity = FVector::ZeroVector;

        // Disable all future collision on the root component
        CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        // Hide the Static Mesh Component that we found in BeginPlay
        if (ProjectileMesh)
        {
            ProjectileMesh->SetVisibility(false, false);
        }

        // Deactivate the trail so it can fade out gracefully
        if (TrailNSC && TrailNSC->IsActive())
        {
            TrailNSC->Deactivate();
        }

        // Set the actor to be destroyed after the linger duration
        SetLifeSpan(LingerDuration);
    }
}