#include "TurretRotationComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h" // UKismetMathLibrary contains MakeRotFromXZ if FRotationMatrix is too much
#include "Math/RotationMatrix.h"     // For FRotationMatrix
#include "DrawDebugHelpers.h"        // Optional: For visualizing the raycast

// Sets default values for this component's properties
UTurretRotationComponent::UTurretRotationComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.
    // You can turn on ticking here if you want the component to manage its own update loop,
    // but for now, we'll keep it so RotateTurretHead is called explicitly from Blueprint.
    PrimaryComponentTick.bCanEverTick = false;

    // bWantsBeginPlay is true by default for USceneComponent, so BeginPlay() will be called.
    // No need to explicitly set bWantsBeginPlay = true; here.
    // REMOVED: bWantsBeginPlay = true; // This was line 17 and causing the error

    // If you specifically wanted to PREVENT BeginPlay from running, you would set:
    // bWantsBeginPlay = false;
}

// Called when the game starts
void UTurretRotationComponent::BeginPlay()
{
    Super::BeginPlay();
    // Your BeginPlay logic here, if any
}

// Helper function (less critical now but can be useful)
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

    // Determine the world space location to aim at
    FVector TargetAimLocationWorld;

    FVector TraceStart = CameraTransform.GetLocation();
    FVector TraceEnd = TraceStart + (CameraTransform.GetRotation().GetForwardVector() * TraceDistance);

    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(OwningActor);

    bool bHit = World->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        CollisionParams
    );

    if (bHit)
    {
        TargetAimLocationWorld = HitResult.ImpactPoint;
    }
    else
    {
        // If no hit, aim towards a point far along the camera's forward vector
        TargetAimLocationWorld = TraceEnd; // Or CameraTransform.GetLocation() + CameraTransform.GetRotation().GetForwardVector() * SomeLargeDistance;
    }

    // Calculate the direction the turret should look
    FVector LookAtDirection = TargetAimLocationWorld - TurretWorldLocation;

    if (LookAtDirection.IsNearlyZero()) // Avoid issues if target is at component's location
    {
        // Fallback: Use the owning actor's forward orientation but maintain its roll.
        // Or, try to match camera's general orientation with owner's roll.
        // For now, let's make it aim along camera's XZ plane projection, with owner's roll.
        FVector CameraForwardXZ = CameraTransform.GetRotation().GetForwardVector();
        CameraForwardXZ.Z = 0; // Project onto XY plane of camera, then use Owner's up.
        if (CameraForwardXZ.IsNearlyZero()) { // If camera looks straight up/down
            CameraForwardXZ = OwningActor->GetActorForwardVector(); // Fallback to owner's forward
        }
        LookAtDirection = CameraForwardXZ;
    }
    
    LookAtDirection.Normalize();


    // Get the owning actor's (vehicle's) up vector to constrain the turret's roll
    FVector VehicleUpVector = OwningActor->GetActorUpVector();
    if (VehicleUpVector.IsNearlyZero()) // Should not happen for a valid actor
    {
        VehicleUpVector = FVector::UpVector; // Absolute fallback
    }
    VehicleUpVector.Normalize();


    // Create the target rotation using MakeFromXZ.
    // This makes the turret's X-axis (Forward) point along LookAtDirection,
    // and its Z-axis (Up) align with VehicleUpVector.
    // This inherently keeps the turret "level" with the vehicle's definition of "up".
    TargetWorldRotationCalculated = FRotationMatrix::MakeFromXZ(LookAtDirection, VehicleUpVector).Rotator();


    FRotator CurrentWorldRotation = GetComponentRotation();

    // Smoothly interpolate the component's current world rotation to the desired target world rotation
    FRotator NewWorldRotation = FMath::RInterpTo(
        CurrentWorldRotation,
        TargetWorldRotationCalculated, // This rotator now has its roll constrained correctly
        DeltaTime,
        InterpolationSpeed
    );

    SetWorldRotation(NewWorldRotation);
}
