#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "UpgradeSubsystem.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Initialize variables
    CurrentBounces = 0;
    MaxBounces = 0; // Bouncing is now superseded by piercing logic
    Damage = 10.0f;
    ImpactForceMagnitude = 100.0f;
    LingerDuration = 3.0f;
    bIsDying = false;
    ProjectilePiercing = 0; // Default to no piercing

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionComp->InitSphereRadius(5.0f);
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    RootComponent = CollisionComp;

    ProjectileMesh = nullptr;

    TrailNSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNSC"));
    TrailNSC->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false; // Disable default bounce behavior

    InitialLifeSpan = 10.0f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    ProjectileMesh = FindComponentByClass<UStaticMeshComponent>();
    if (!ProjectileMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("AProjectile (%s) could not find a UStaticMeshComponent."), *GetNameSafe(this));
    }

    if (TrailNiagaraSystemAsset)
    {
        TrailNSC->SetAsset(TrailNiagaraSystemAsset);
        TrailNSC->Activate(true);
    }

    //add additional damage
    if (!GetUpgradeSubsystem())
    {
        return;
    }

    Damage += GetUpgradeSubsystem()->GetUpgradeValue(EUpgradeType::TurretDamage);
}

void AProjectile::InitializeProjectile(float InAdditionalDamage, int32 InPiercingCount, float InSpeedMultiplier)
{
    // Apply stats passed in from the weapon that fired this projectile.
    Damage += InAdditionalDamage;
    ProjectilePiercing = InPiercingCount;
    
    ProjectileMovement->InitialSpeed *= InSpeedMultiplier;
    ProjectileMovement->MaxSpeed *= InSpeedMultiplier;
    ProjectileMovement->Velocity = GetActorForwardVector() * ProjectileMovement->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bIsDying || !OtherActor || OtherActor == this || OtherActor == GetInstigator())
    {
        return;
    }

    //make sure we have an upgrade subsystem
    if (!GetUpgradeSubsystem())
    {
        return;
    }

    //grab the crit chance
    float CritChance = GetUpgradeSubsystem()->GetUpgradeValue(EUpgradeType::TurretCritChance);

    //should this be a crit?
    if (CritChance >= FMath::RandRange(0.01f, 100.0f))
    {
	    //add the crit damage
        Damage += GetUpgradeSubsystem()->GetUpgradeValue(EUpgradeType::TurretCritDamage);
    }


    // Apply damage and impulse first, regardless of what was hit.
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

    // Broadcast the event so Blueprints (like this projectile's own) can react.
    OnProjectileImpact.Broadcast(Hit.ImpactPoint, OtherActor);

    // --- PIERCING LOGIC ---
    // Check if the hit actor is an enemy.
    if (OtherActor->ActorHasTag(FName("Enemy")))
    {
        if (ProjectilePiercing > 0)
        {
            // If we can still pierce, decrement the count and continue flying.
            ProjectilePiercing--;
            return; // Exit the OnHit function here to allow the projectile to continue.
        }
    }

    // --- END OF LIFE LOGIC ---
    // This code will only run if the projectile hit something that wasn't an enemy,
    // or if it hit an enemy but had no piercing left.
    bIsDying = true;
    ProjectileMovement->StopMovementImmediately();
    ProjectileMovement->Velocity = FVector::ZeroVector;
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (ProjectileMesh)
    {
        ProjectileMesh->SetVisibility(false, false);
    }

    if (TrailNSC && TrailNSC->IsActive())
    {
        TrailNSC->Deactivate();
    }

    SetLifeSpan(LingerDuration);
}

UUpgradeSubsystem* AProjectile::GetUpgradeSubsystem()
{
    //if we have it already, return it
	if (UpgradeSubsystem)
	{
        return UpgradeSubsystem;
	}

    //grab it
    UpgradeSubsystem = GetGameInstance()->GetSubsystem<UUpgradeSubsystem>();

    //return it
    return UpgradeSubsystem;
}
