#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"   // For UNiagaraComponent
#include "NiagaraSystem.h"      // For UNiagaraSystem
#include "NiagaraFunctionLibrary.h" // For spawning Niagara systems if needed, though here we use a component

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
	CollisionComp->SetHiddenInGame(true);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.6f;

	// Create and setup the Niagara system component for the trail
	TrailNSC = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNSC"));
	TrailNSC->SetupAttachment(RootComponent);
	// TrailNSC->bAutoActivate = false; // We will activate in BeginPlay after the asset is set

	// The TrailNiagaraSystemAsset will be assigned in the Blueprint.
	// UNiagaraComponent uses SetAsset() to assign the system.

	InitialLifeSpan = 3.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Set the Niagara system asset for the component if it has been assigned (e.g., in Blueprint)
	if (TrailNiagaraSystemAsset)
	{
		TrailNSC->SetAsset(TrailNiagaraSystemAsset);
		TrailNSC->Activate(true); // Activate the system. 'true' for reset.
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AProjectile (%s) has no TrailNiagaraSystemAsset assigned for its TrailNSC."), *GetName());
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr))
	{
		return;
	}

	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
	}

	bool bShouldDestroy = false;
	if (ProjectileMovement->bShouldBounce)
	{
		CurrentBounces++;
		if (CurrentBounces >= MaxBounces)
		{
			bShouldDestroy = true;
		}
	}
	else
	{
		bShouldDestroy = true;
	}

	if (bShouldDestroy)
	{
		if (TrailNSC && TrailNSC->IsActive())
		{
			TrailNSC->Deactivate(); // Deactivate gracefully
			// For Niagara, you might also consider TrailNSC->SetVisibility(false); or other ways to manage particles on destruction.
		}
		SetLifeSpan(0.1f); // Destroy quickly after deactivating particles
	}
}