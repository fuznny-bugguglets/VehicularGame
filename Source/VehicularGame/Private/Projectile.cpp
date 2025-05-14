#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h" // Required for UNiagaraSystem type if you use GetAsset()
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(SphereCastRadius);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));

    BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    BulletMeshComponent->SetupAttachment(RootComponent);
    BulletMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    TrailPSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailPSC")); // <--- CHANGED TYPE HERE
    TrailPSC->SetupAttachment(RootComponent);

    IgnoredTags.Add(TEXT("Bullet"));
    IgnoredTags.Add(TEXT("Player"));

    EnemyCollisionChannel = ECC_Pawn; // Default, configure as needed

    InitialLifeSpan = MaxLifetime;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    InitializeMovement();

    if (CollisionComponent)
    {
        CollisionComponent->SetSphereRadius(SphereCastRadius); // Ensure radius matches property
    }

    // Example: Setup dynamic material instance for trail fading
    // This requires your trail particle system's material to be set up for dynamic parameters.
    // if (TrailPSC && TrailPSC->GetNumMaterials() > 0 && TrailPSC->GetMaterial(0) != nullptr)
    // {
    //     TrailMaterialDynamic = UMaterialInstanceDynamic::Create(TrailPSC->GetMaterial(0), this);
    //     if (TrailMaterialDynamic)
    //     {
    //         TrailPSC->SetMaterial(0, TrailMaterialDynamic);
    //         // You might want to get initial alpha values here if they are part of the material
    //         // TrailMaterialDynamic->GetScalarParameterValue(FName("AlphaScale"), InitialTrailStartAlpha);
    //     }
    // }
}

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bHasExploded)
    {
        UpdateTrailFade(DeltaTime);
    }
    else
    {
        UpdateMovement(DeltaTime);
    }

#if WITH_EDITOR
    if (GetWorld() && GetWorld()->IsGameWorld())
    {
        DrawDebugShapes();
    }
#endif
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

void AProjectile::InitializeMovement()
{
    CurrentVelocity = GetActorForwardVector() * InitialSpeed;
}

void AProjectile::UpdateMovement(float DeltaTime)
{
    FVector StartLocation = GetActorLocation();

    if (GravityMultiplier > 0.0f)
    {
        CurrentVelocity += FVector(0.0f, 0.0f, GetWorld()->GetGravityZ()) * GravityMultiplier * DeltaTime;
    }

    FVector MoveDelta = CurrentVelocity * DeltaTime;
    FVector EndLocation = StartLocation + MoveDelta;

    if (!CurrentVelocity.IsNearlyZero())
    {
        SetActorRotation(CurrentVelocity.ToOrientationRotator());
    }

    FHitResult HitResult;
    if (PerformSweepCheck(StartLocation, EndLocation, SphereCastRadius, HitResult))
    {
        SetActorLocation(HitResult.ImpactPoint);
        ProcessHit(HitResult);
    }
    else
    {
        SetActorLocation(EndLocation);
    }
}

bool AProjectile::PerformSweepCheck(const FVector& StartLocation, const FVector& EndLocation, float SweepRadius, FHitResult& OutHit)
{
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    QueryParams.bTraceComplex = true; // Consider performance if not needed

    return GetWorld()->SweepSingleByChannel(
        OutHit,
        StartLocation,
        EndLocation,
        FQuat::Identity,
        CollisionComponent->GetCollisionObjectType(), // Use projectile's collision channel for the sweep
        FCollisionShape::MakeSphere(SweepRadius),
        QueryParams
    );
}

bool AProjectile::ShouldIgnoreHit(const AActor* HitActor, const UPrimitiveComponent* HitComponent) const
{
    if (!HitActor) return true;

    for (const FName& Tag : IgnoredTags)
    {
        if (HitActor->ActorHasTag(Tag))
        {
            return true;
        }
    }
    return false;
}

void AProjectile::ProcessHit(const FHitResult& Hit)
{
    if (bHasExploded) return; // Don't process if already exploded

    AActor* HitActor = Hit.GetActor();
    UPrimitiveComponent* HitComponent = Hit.GetComponent();

    if (ShouldIgnoreHit(HitActor, HitComponent))
    {
        // Note: With SweepSingle, an ignored hit still stops the sweep.
        // If you need to pass through ignored objects and hit what's behind,
        // you'd use SweepMulti, iterate, and find the first non-ignored hit.
        // For this implementation, an ignored hit effectively means no further processing for *this* hit.
        return;
    }

    CollisionCount++;

    bool bIsEnemy = HitActor && HitActor->ActorHasTag(TEXT("Enemy")); // Example "Enemy" tag check

    PlayHitEffect(Hit.ImpactPoint, Hit.ImpactNormal, bIsEnemy);

    if ((bIsEnemy && bExplodeOnTouch) || CollisionCount >= MaxCollisionsBeforeExplode)
    {
        Explode();
    }
    else // Ricochet logic
    {
        // Reflect velocity (simple reflection, preserves speed)
        CurrentVelocity = FMath::GetReflectionVector(CurrentVelocity, Hit.ImpactNormal);
        // The Unity script's Y-velocity preservation during reflection is more complex and
        // physics-simulation dependent. A direct FMath::GetReflectionVector is common for projectiles.
        // If you need the exact Y-preservation from the script, that part would need custom decomposition and reconstruction of the velocity vector.
        SetActorRotation(CurrentVelocity.ToOrientationRotator());
    }
}

void AProjectile::PlayHitEffect(const FVector& ImpactPoint, const FVector& ImpactNormal, bool bIsEnemy)
{
    UParticleSystem* EffectToPlay = bIsEnemy ? EnemyHitParticles : WallHitParticles;
    if (EffectToPlay)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            EffectToPlay,
            ImpactPoint,
            ImpactNormal.Rotation(),
            true // Auto-destroy emitter
        );
    }
}

void AProjectile::Explode()
{
    if (bHasExploded) return;
    bHasExploded = true;
    TimeSinceExploded = 0.0f;

    if (BulletMeshComponent) BulletMeshComponent->SetVisibility(false, true);
    if (CollisionComponent) CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CurrentVelocity = FVector::ZeroVector;

    if (ExplosionEffect) // This is still UParticleSystem, assuming explosion is Cascade or you change its type
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), true);
    }

    TArray<AActor*> OverlappedActors;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(EnemyCollisionChannel));

    TArray<AActor*> ActorsToIgnoreForOverlap;
    // ActorsToIgnoreForOverlap.Add(this); // Usually not needed as SphereOverlapActors often ignores self by default

    UKismetSystemLibrary::SphereOverlapActors(
        GetWorld(),
        GetActorLocation(),
        ExplosionRange,
        ObjectTypes,
        nullptr, 
        ActorsToIgnoreForOverlap,
        OverlappedActors
    );

    for (AActor* OverlappedActor : OverlappedActors)
    {
        if (!OverlappedActor || OverlappedActor == this) continue;

        UGameplayStatics::ApplyRadialDamage(
            GetWorld(),
            ExplosionDamage,
            GetActorLocation(),
            ExplosionRange,
            UDamageType::StaticClass(), 
            TArray<AActor*>(), 
            this,              
            GetInstigatorController(), 
            true,              
            ECC_Visibility     
        );

        UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OverlappedActor->GetRootComponent());
        if (PrimComp && PrimComp->IsSimulatingPhysics())
        {
            PrimComp->AddRadialForce(GetActorLocation(), ExplosionRange, ExplosionForce, ERadialImpulseFalloff::RIF_Linear, true);
        }
    }

    // Determine lifetime for trail fade
    // For Niagara, getting an exact "duration" is complex. Let's use a configurable/default value.
    // You can add a UPROPERTY: float ActualTrailEffectDuration = 2.0f; and set it in Blueprint.
    // For now, using a fixed value or a value derived from TrailFadeOutSpeed if it's meant to be a rate.
    float TrailEffectLiveTime = 2.0f; // Default time for trail to be visible after explosion
    if (TrailFadeOutSpeed > KINDA_SMALL_NUMBER) {
        // If TrailFadeOutSpeed is treated as a rate, this isn't direct.
        // If it implies a duration, you might use it. Let's assume a fixed time for now.
    }
    
    // Ensure the actor lives long enough for the trail to be handled by UpdateTrailFade or just deactivate.
    SetLifeSpan(TrailEffectLiveTime); 
}

void AProjectile::UpdateTrailFade(float DeltaTime)
{
    TimeSinceExploded += DeltaTime;

    // Option 1: Control via Material (Complex for Niagara without specific material setup)
    // if (TrailPSC && TrailMaterialDynamic) 
    // {
    //    ... (material parameter fading logic) ...
    // }
    // Option 2: Simply deactivate the Niagara component and let particles finish their lifetime.
    // The actor's overall LifeSpan set in Explode() will handle destroying the actor later.
    if (TrailPSC && TrailPSC->IsActive()) 
    {
        // Deactivate after a very short delay to ensure any post-explosion spawn logic within Niagara has fired.
        // Or, if your Niagara system is designed to stop emitting on a parameter change, you'd set that parameter here.
        if (TimeSinceExploded > 0.05f) // Small delay
        {
            TrailPSC->Deactivate(); // <--- CORRECTED: Use Deactivate() for UNiagaraComponent
        }
    }
}

#if WITH_EDITOR
void AProjectile::DrawDebugShapes() const
{
    DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRange, 24, FColor::Red, false, -1.0f, 0, 0.5f);
    DrawDebugSphere(GetWorld(), GetActorLocation(), SphereCastRadius, 12, FColor::Yellow, false, -1.0f, 0, 0.5f);

    if (!CurrentVelocity.IsNearlyZero())
    {
        DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + CurrentVelocity.GetSafeNormal() * 200.0f, FColor::Blue, false, -1.0f, 0, 0.5f);
    }
}
#endif