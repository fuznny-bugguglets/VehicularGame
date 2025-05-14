#include "TurretRotationComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RotationMatrix.h" // For FRotationMatrix.
#include "DrawDebugHelpers.h"

// Constructor.
UTurretRotationComponent::UTurretRotationComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Component does not tick by itself.
    // BeginPlay is called by default for USceneComponent.
}

// Called when the game starts.
void UTurretRotationComponent::BeginPlay()
{
    Super::BeginPlay();
}

AActor* UTurretRotationComponent::GetOwningActor() const
{
    return GetOwner();
}

void UTurretRotationComponent::RotateTurretHead(const FTransform& CameraTransform, float DeltaTime, float TraceDistance, float InterpolationSpeed)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("TurretRotationComponent cannot get World."));
        return;
    }

    AActor* OwningActor = GetOwner();
    if (!OwningActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("TurretRotationComponent has no Owning Actor to align with."));
        return;
    }

    FVector TurretWorldLocation = GetComponentLocation();
    FRotator TargetWorldRotationCalculated;

    // Determine target aim location in world space.
    FVector TargetAimLocationWorld;

    FVector TraceStart = CameraTransform.GetLocation();
    FVector TraceEnd = TraceStart + (CameraTransform.GetRotation().GetForwardVector() * TraceDistance);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwningActor); // Ignore the owning actor in raycast.

    bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        CollisionParams
    );

    if (bHit)
    {
        TargetAimLocationWorld = HitResult.ImpactPoint; // Aim at the raycast hit point.
    }
    else
    {
        // If no raycast hit, aim along camera's forward vector.
        TargetAimLocationWorld = TraceEnd;
    }

    // Calculate look direction.
    FVector LookAtDirection = TargetAimLocationWorld - TurretWorldLocation;

    if (LookAtDirection.IsNearlyZero()) // Handle case where target is at component's location.
    {
        FVector CameraForwardXZ = CameraTransform.GetRotation().GetForwardVector();
        CameraForwardXZ.Z = 0; // Project camera forward vector onto XY plane.
        if (CameraForwardXZ.IsNearlyZero()) { // Handle camera looking straight up/down.
            CameraForwardXZ = OwningActor->GetActorForwardVector(); // Fallback to owner's forward vector.
        }
        LookAtDirection = CameraForwardXZ;
    }
    
    LookAtDirection.Normalize();


    // Get owner's up vector to constrain turret roll.
    FVector VehicleUpVector = OwningActor->GetActorUpVector();
    if (VehicleUpVector.IsNearlyZero()) // Handle invalid owner up vector.
    {
        VehicleUpVector = FVector::UpVector; // Use world up vector as fallback.
    }
    VehicleUpVector.Normalize();


    // Create target rotation, aligning X to LookAtDirection and Z to VehicleUpVector.
    TargetWorldRotationCalculated = FRotationMatrix::MakeFromXZ(LookAtDirection, VehicleUpVector).Rotator();


    FRotator CurrentWorldRotation = GetComponentRotation();

    // Interpolate to target rotation.
    FRotator NewWorldRotation = FMath::RInterpTo(
        CurrentWorldRotation,
        TargetWorldRotationCalculated,
        DeltaTime,
        InterpolationSpeed
    );

    SetWorldRotation(NewWorldRotation);
}