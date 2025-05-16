#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h" // For UGameplayStatics if needed, not strictly for this logic

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CurrentBounces = 0;
    MaxBounces = 1; // Projectile will be destroyed on the first impact that counts (after CurrentBounces becomes >= MaxBounces)
    // If you want it to survive one bounce and destroy on the *second* hit, set MaxBounces = 2.
    // If MaxBounces = 0, it means it should be destroyed on the very first impact (CurrentBounces will be 1, 1 >= 0 -> destroy)
    // Based on your request, "destroy immediately", MaxBounces = 1 (destroy on first valid hit) or MaxBounces = 0 (same practical outcome) is good.

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile"); //
    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit); //
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f)); //
    CollisionComp->CanCharacterStepUpOn = ECB_No; //
    // CollisionComp->SetHiddenInGame(true); // Keep this as you prefer for debugging/release
    RootComponent = CollisionComp; //

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp")); //
    ProjectileMovement->UpdatedComponent = CollisionComp; //
    ProjectileMovement->InitialSpeed = 3000.f; //
    ProjectileMovement->MaxSpeed = 3000.f; //
    ProjectileMovement->bRotationFollowsVelocity = true; //
    ProjectileMovement->bShouldBounce = true; // Set to true if you want bouncing behavior generally
    // If you *never* want it to bounce, set this to false.
    // Our OnHit logic will override for immediate destruction.
    ProjectileMovement->Bounciness = 0.6f; //

    TrailNSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNSC")); //
    TrailNSC->SetupAttachment(RootComponent); //
    // TrailNSC->bAutoActivate = false; // We activate in BeginPlay

    InitialLifeSpan = 3.0f; //
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (TrailNiagaraSystemAsset) //
    {
        TrailNSC->SetAsset(TrailNiagaraSystemAsset); //
        TrailNSC->Activate(true); //
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AProjectile (%s) has no TrailNiagaraSystemAsset assigned for its TrailNSC."), *GetName()); //
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Ignore self, invalid actors, or the instigator (e.g. player's vehicle that fired it)
    if (!OtherActor || OtherActor == this || !OtherComp || OtherActor == GetInstigator())
    {
        return;
    }

    // Apply physics impulse to the hit object if it's simulating physics
    if (OtherComp->IsSimulatingPhysics()) //
    {
        OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint); //
    }

    CurrentBounces++; //
    bool bShouldBeDestroyed = false;

    // Determine if the projectile should be destroyed on this hit
    // If bShouldBounce is false, it always destroys.
    // If bShouldBounce is true, it destroys if CurrentBounces >= MaxBounces.
    // Note: MaxBounces = 0 or MaxBounces = 1 will lead to destruction on the first valid impact.
    if (!ProjectileMovement->bShouldBounce || (CurrentBounces >= MaxBounces)) //
    {
        bShouldBeDestroyed = true; //
    }

    if (bShouldBeDestroyed)
    {
        // Stop movement and disable collision immediately to prevent any further bounce or interaction.
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Velocity = FVector::ZeroVector; // Ensure it's fully stopped
        SetActorEnableCollision(false); // Disable all collision for this actor

        // Handle the trail effect
        if (TrailNSC && TrailNSC->IsActive()) //
        {
            TrailNSC->Deactivate(); // Tell the system to stop spawning new particles
            // Detach the component from the projectile actor so it persists independently.
            // It will auto-destroy when its particles have finished.
            TrailNSC->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        }

        // Destroy the projectile actor itself
        Destroy();
    }
    else
    {
        // If we are not destroying it, and bShouldBounce is true, the ProjectileMovementComponent will handle the bounce.
        // You can add sounds or other effects for bounces here if needed.
    }
}