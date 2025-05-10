#include "CustomWheelComponent.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

UCustomWheelComponent::UCustomWheelComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics; // Tick after physics simulation.

    WheelRadius = 35.0f;
    SuspensionMaxLength = 15.0f;
    WheelMass = 15.0f;
    SuspensionForce = 25000.0f;
    SuspensionDamping = 2500.0f;
    SuspensionTargetPosition = 0.5f;
    bDrawDebug = true;
    WheelAngularVelocity = 0.0f;
    ForwardSlip = 0.0f;
    SidewaysSlip = 0.0f;
    MotorTorqueInput = 0.0f;
    BrakeTorqueInput = 0.0f;
    SteerAngle = 0.0f;

    bEditableWhenInherited = true;

    SmoothingAngVelThreshold = 50.0f;
    MaxSmoothingInterpSpeed = 5.0f;
    MinSmoothingInterpSpeed = 50.0f;

    FrictionReductionStartAngle = 30.0f;
    FrictionReductionMaxAngle = 60.0f;
    MinStiffnessMultiplierAtMaxAngle = 0.5f;

    SmoothedForwardFrictionForce = FVector::ZeroVector;
    SmoothedSidewaysFrictionForce = FVector::ZeroVector;
    SmoothedFrictionTorque = 0.0f;
}

void UCustomWheelComponent::SetSteerAngle(float Angle)
{
    SteerAngle = FMath::Clamp(Angle, -90.0f, 90.0f); // Clamp steer angle.
}

void UCustomWheelComponent::BeginPlay()
{
    Super::BeginPlay();

    // Get visual mesh and owner's physics body.
    TArray<USceneComponent*> Children;
    GetChildrenComponents(false, Children);
    if (Children.Num() > 0) { VisualWheelMesh = Children[0]; }
    AActor* Owner = GetOwner();
    if (Owner) { OwningPrimitiveComponent = Cast<UPrimitiveComponent>(Owner->GetRootComponent()); }

    // Initialize previous frame's state.
    PreviousSuspensionCompression = SuspensionCompression;
    PreviousPosition = GetComponentLocation();

    // Ensure friction curves are built.
    ForwardFrictionCurve.UpdateArrays();
    SidewaysFrictionCurve.UpdateArrays();

    // Calculate wheel inertia.
    WheelInertia = 0.5f * WheelMass * FMath::Square(WheelRadius);
    if (WheelInertia < KINDA_SMALL_NUMBER) // Prevent division by zero.
    {
        UE_LOG(LogTemp, Warning, TEXT("CustomWheelComponent '%s' has near-zero Inertia. Clamping."), *GetName());
        WheelInertia = 0.1f; // Assign minimum inertia.
    }

    SmoothedForwardFrictionForce = FVector::ZeroVector;
    SmoothedSidewaysFrictionForce = FVector::ZeroVector;
    SmoothedFrictionTorque = 0.0f;
}

float UCustomWheelComponent::GetCurrentSmoothingInterpSpeed() const
{
    if (SmoothingAngVelThreshold <= KINDA_SMALL_NUMBER)
    {
        return MinSmoothingInterpSpeed; // Avoid division by zero.
    }
    // Alpha: 1.0 at zero speed, 0.0 above threshold.
    const float Alpha = 1.0f - FMath::Clamp(FMath::Abs(WheelAngularVelocity) / SmoothingAngVelThreshold, 0.0f, 1.0f);
    // Interpolate between min and max smoothing speeds.
    return FMath::Lerp(MinSmoothingInterpSpeed, MaxSmoothingInterpSpeed, Alpha);
}


void UCustomWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!OwningPrimitiveComponent) // Ensure owner's physics body is valid.
    {
        AActor* CurrentOwner = GetOwner();
        if (CurrentOwner) { OwningPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentOwner->GetRootComponent()); }
        if (!OwningPrimitiveComponent) { return; } // Cannot proceed.
    }

    PreviousSuspensionCompression = SuspensionCompression; // Store for damping.

    UpdateSuspension(DeltaTime);        // Update suspension state.
    CalculateSlips(DeltaTime);          // Calculate tire slips.
    UpdateWheelPhysics(DeltaTime);      // Update wheel rotational physics.

    WheelRotationAngle -= WheelAngularVelocity * DeltaTime; // Update visual rotation angle.

    CalculateAndApplyForces(DeltaTime); // Calculate and apply forces.
    UpdateWheelVisuals(DeltaTime);      // Update visual mesh.
    PreviousPosition = GetComponentLocation(); // Store current position.
}

void UCustomWheelComponent::UpdateSuspension(float DeltaTime)
{
    // Suspension raycast start and end points.
    const FVector ComponentUp = GetUpVector();
    const FVector StartLocation = GetComponentLocation();
    const float TraceLength = SuspensionMaxLength + WheelRadius;
    const FVector EndLocation = StartLocation - ComponentUp * TraceLength;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CustomWheelTrace), false, GetOwner());
    QueryParams.bTraceComplex = true; // Use complex collision.

    bool bHit = GetWorld()->LineTraceSingleByChannel(CurrentHitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
    bIsGrounded = bHit;

    if (bIsGrounded) {
        SuspensionCompression = FMath::Clamp(SuspensionMaxLength - (CurrentHitResult.Distance - WheelRadius), 0.0f, SuspensionMaxLength); // Calculate compression.
        if (bDrawDebug) { DrawDebugLine(GetWorld(), StartLocation, CurrentHitResult.ImpactPoint, FColor::Green, false, -1.f, 0, 0.5f); DrawDebugSphere(GetWorld(), CurrentHitResult.ImpactPoint, 5.f, 8, FColor::Green, false, -1.f, 0, 0.5f); }
    }
    else {
        SuspensionCompression = 0.0f; // No compression.
        CurrentHitResult.Reset();     // Clear hit result.
        if (bDrawDebug) { DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 0.5f); }
    }
}

void UCustomWheelComponent::CalculateSlips(float DeltaTime)
{
    if (!bIsGrounded || DeltaTime <= KINDA_SMALL_NUMBER)
    {
        ForwardSlip = 0.0f; // Reset slips.
        SidewaysSlip = 0.0f;
        return;
    }

    // Linear velocity of wheel's contact point.
    const FVector CurrentPosition = GetComponentLocation();
    const FVector Velocity = (CurrentPosition - PreviousPosition) / DeltaTime;

    // Wheel's local axes with steer angle.
    const FQuat ComponentRotation = GetComponentQuat();
    const FVector BaseForward = ComponentRotation.GetForwardVector();
    const FVector BaseRight = ComponentRotation.GetRightVector();
    const FVector BaseUp = ComponentRotation.GetUpVector();
    const FQuat SteerRotation = FQuat(BaseUp, FMath::DegreesToRadians(SteerAngle)); // Steer rotation.
    const FVector SteeredForward = SteerRotation.RotateVector(BaseForward);
    const FVector SteeredSideways = SteerRotation.RotateVector(-BaseRight); // Sideways is opposite to steered right.

    // Project wheel velocity onto steered axes.
    const float ForwardVelocityComponent = FVector::DotProduct(Velocity, SteeredForward);
    const float SidewaysVelocityComponent = FVector::DotProduct(Velocity, SteeredSideways);

    // Wheel's surface speed from rotation.
    const float WheelSurfaceSpeedRad = FMath::DegreesToRadians(WheelAngularVelocity);
    const float WheelSurfaceSpeed = WheelSurfaceSpeedRad * WheelRadius;

    // Calculate slips.
    // ForwardSlip: Difference between wheel surface speed and ground speed in forward direction.
    // Positive slip: wheel spins faster than ground (acceleration).
    // Negative slip: wheel spins slower than ground (braking/deceleration).
    ForwardSlip = WheelSurfaceSpeed - ForwardVelocityComponent;
    // SidewaysSlip: Ground speed in the sideways direction.
    SidewaysSlip = -SidewaysVelocityComponent; // Negative due to SteeredSideways definition.
}

void UCustomWheelComponent::UpdateWheelPhysics(float DeltaTime)
{
    if (DeltaTime <= KINDA_SMALL_NUMBER || WheelInertia <= KINDA_SMALL_NUMBER) return;

    // Apply motor torque.
    float MotorAccelRad = MotorTorqueInput / WheelInertia; // Angular acceleration.
    WheelAngularVelocity += FMath::RadiansToDegrees(MotorAccelRad * DeltaTime); // Update angular velocity.

    // Apply brake torque.
    if (BrakeTorqueInput > KINDA_SMALL_NUMBER) {
        float MaxBrakeDecelRad = BrakeTorqueInput / WheelInertia;
        float DeltaVelBrakeRad = MaxBrakeDecelRad * DeltaTime;
        float CurrentAngVelRad = FMath::DegreesToRadians(WheelAngularVelocity);
        float ActualDeltaVelBrakeRad = FMath::Min(FMath::Abs(CurrentAngVelRad), DeltaVelBrakeRad); // Prevent brake from reversing direction.
        WheelAngularVelocity -= FMath::Sign(CurrentAngVelRad) * FMath::RadiansToDegrees(ActualDeltaVelBrakeRad);
    }

    // Calculate and apply smoothed friction torque.
    if (bIsGrounded)
    {
        const float ModifiedForwardMagnitude = GetModifiedForwardFrictionMagnitude(); // Magnitude including slope effects.
        const float SignedForwardForce = FMath::Sign(ForwardSlip) * ModifiedForwardMagnitude;
        const float RawFrictionTorque = -SignedForwardForce * WheelRadius; // Torque opposes relative motion.

        const float CurrentInterpSpeed = GetCurrentSmoothingInterpSpeed(); // Interpolation speed.
        SmoothedFrictionTorque = FMath::FInterpTo(SmoothedFrictionTorque, RawFrictionTorque, DeltaTime, CurrentInterpSpeed); // Smooth torque.

        const float SmoothedFrictionAccelRad = SmoothedFrictionTorque / WheelInertia; // Acceleration from friction.
        WheelAngularVelocity += FMath::RadiansToDegrees(SmoothedFrictionAccelRad * DeltaTime); // Apply friction effect.
    }
    else
    {
        SmoothedFrictionTorque = 0.0f; // Reset torque if not grounded.
    }
}

void UCustomWheelComponent::CalculateAndApplyForces(float DeltaTime)
{
    if (bIsGrounded && OwningPrimitiveComponent && OwningPrimitiveComponent->IsSimulatingPhysics())
    {
        // Calculate suspension forces.
        const FVector ComponentUp = GetUpVector();
        const float SpringForceMagnitude = (SuspensionCompression - (SuspensionMaxLength * SuspensionTargetPosition)) * SuspensionForce;
        float DampingForceMagnitude = 0.0f;
        if (DeltaTime > KINDA_SMALL_NUMBER) {
            const float CompressionVelocity = (SuspensionCompression - PreviousSuspensionCompression) / DeltaTime;
            DampingForceMagnitude = CompressionVelocity * SuspensionDamping;
        }
        const FVector SuspensionForceVector = ComponentUp * (SpringForceMagnitude + DampingForceMagnitude);

        // Calculate tire friction forces.
        const FQuat ComponentRotation = GetComponentQuat();
        const FVector BaseForward = ComponentRotation.GetForwardVector();
        const FVector BaseRight = ComponentRotation.GetRightVector();
        const FVector BaseUp = ComponentRotation.GetUpVector();
        const FQuat SteerRotation = FQuat(BaseUp, FMath::DegreesToRadians(SteerAngle));
        const FVector SteeredForward = SteerRotation.RotateVector(BaseForward);
        const FVector SteeredRight = SteerRotation.RotateVector(BaseRight);

        // Get friction magnitudes including slope effects.
        const float ModifiedForwardMag = GetModifiedForwardFrictionMagnitude();
        const float ModifiedSidewaysMag = GetModifiedSidewaysFrictionMagnitude();

        const float RawForwardForceMagSigned = FMath::Sign(ForwardSlip) * ModifiedForwardMag;
        const FVector RawForwardForceVector = SteeredForward * RawForwardForceMagSigned;

        const float RawSidewaysForceMagSigned = FMath::Sign(SidewaysSlip) * ModifiedSidewaysMag;
        const FVector RawSidewaysForceVector = -SteeredRight * RawSidewaysForceMagSigned; // Force opposes sideways slip.

        const float CurrentInterpSpeed = GetCurrentSmoothingInterpSpeed(); // Interpolation speed.
        SmoothedForwardFrictionForce = FMath::VInterpTo(SmoothedForwardFrictionForce, RawForwardForceVector, DeltaTime, CurrentInterpSpeed); // Smooth forces.
        SmoothedSidewaysFrictionForce = FMath::VInterpTo(SmoothedSidewaysFrictionForce, RawSidewaysForceVector, DeltaTime, CurrentInterpSpeed);

        const FVector TotalForce = SuspensionForceVector + SmoothedForwardFrictionForce + SmoothedSidewaysFrictionForce; // Combine forces.
        const FVector ApplyLocation = CurrentHitResult.ImpactPoint; // Apply at contact point.
        OwningPrimitiveComponent->AddForceAtLocation(TotalForce, ApplyLocation);

        if (bDrawDebug) {
            const float DebugForceScale = 0.01f;
            DrawDebugLine(GetWorld(), ApplyLocation, ApplyLocation + SuspensionForceVector.GetClampedToMaxSize(20000.0f) * DebugForceScale, FColor::Blue, false, -1.f, 0, 1.0f);
            DrawDebugLine(GetWorld(), ApplyLocation, ApplyLocation + SmoothedForwardFrictionForce.GetClampedToMaxSize(20000.0f) * DebugForceScale, FColor(255,128,0), false, -1.f, 0, 1.0f);
            DrawDebugLine(GetWorld(), ApplyLocation, ApplyLocation + SmoothedSidewaysFrictionForce.GetClampedToMaxSize(20000.0f) * DebugForceScale, FColor::Yellow, false, -1.f, 0, 1.0f);
        }
    }
    else
    {
        // Reset smoothed forces if not grounded.
        SmoothedForwardFrictionForce = FVector::ZeroVector;
        SmoothedSidewaysFrictionForce = FVector::ZeroVector;
    }
}

// Calculates forward friction magnitude, including slope effects.
float UCustomWheelComponent::GetModifiedForwardFrictionMagnitude() const
{
    if (!bIsGrounded) return 0.0f;
    const float BaseForce = ForwardFrictionCurve.Evaluate(ForwardSlip);
    const float SlopeMultiplier = GetCurrentSlopeStiffnessMultiplier();
    return BaseForce * SlopeMultiplier;
}

// Calculates sideways friction magnitude, including slope effects.
float UCustomWheelComponent::GetModifiedSidewaysFrictionMagnitude() const
{
    if (!bIsGrounded) return 0.0f;
    const float BaseForce = SidewaysFrictionCurve.Evaluate(SidewaysSlip);
    const float SlopeMultiplier = GetCurrentSlopeStiffnessMultiplier();
    return BaseForce * SlopeMultiplier;
}

// Calculates friction stiffness multiplier based on slope.
float UCustomWheelComponent::GetCurrentSlopeStiffnessMultiplier() const
{
    if (!bIsGrounded || CurrentHitResult.ImpactNormal.IsNearlyZero())
    {
        return 1.0f; // Full stiffness if not grounded or normal is invalid.
    }

    const FVector WorldUp = FVector::UpVector; // World up.
    const FVector SurfaceNormal = CurrentHitResult.ImpactNormal.GetSafeNormal(); // Surface normal.

    // Angle between surface normal and world up.
    const float CosAngle = FVector::DotProduct(SurfaceNormal, WorldUp);
    const float AngleRad = FMath::Acos(FMath::Clamp(CosAngle, -1.0f, 1.0f)); // Clamp for Acos.
    const float AngleDeg = FMath::RadiansToDegrees(AngleRad);

    if (AngleDeg <= FrictionReductionStartAngle) // Below start angle, full stiffness.
    {
        return 1.0f;
    }
    else if (AngleDeg >= FrictionReductionMaxAngle || FrictionReductionMaxAngle <= FrictionReductionStartAngle) // At or beyond max angle, min stiffness.
    {
        return MinStiffnessMultiplierAtMaxAngle;
    }
    else // Linearly interpolate stiffness.
    {
        const float Alpha = (AngleDeg - FrictionReductionStartAngle) / (FrictionReductionMaxAngle - FrictionReductionStartAngle); // Interpolation alpha.
        return FMath::Lerp(1.0f, MinStiffnessMultiplierAtMaxAngle, Alpha); // Interpolate.
    }
}


void UCustomWheelComponent::UpdateWheelVisuals(float DeltaTime)
{
    if (!VisualWheelMesh) return;

    // Update visual wheel position from suspension.
    const float WheelZOffset = -(SuspensionMaxLength - SuspensionCompression); // Negative for upward compression.
    FVector CurrentRelativeLocation = VisualWheelMesh->GetRelativeLocation();
    VisualWheelMesh->SetRelativeLocation(FVector(CurrentRelativeLocation.X, CurrentRelativeLocation.Y, WheelZOffset)); // Assuming Z is suspension axis.

    // Combined rotation (steering and spin).
    const FQuat SteerQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(SteerAngle));       // Steering rotation.
    const FQuat SpinQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(WheelRotationAngle)); // Spin rotation.
    VisualWheelMesh->SetRelativeRotation(SteerQuat * SpinQuat); // Apply combined rotation.
}

void UCustomWheelComponent::UpdateForwardFrictionCurveRuntime()
{
    ForwardFrictionCurve.UpdateArrays();
}

void UCustomWheelComponent::UpdateSidewaysFrictionCurveRuntime()
{
    SidewaysFrictionCurve.UpdateArrays();
}

void UCustomWheelComponent::SetForwardFrictionProperties(const FWheelFrictionCurve& NewForwardFriction)
{
    ForwardFrictionCurve = NewForwardFriction;
    ForwardFrictionCurve.UpdateArrays();
}

void UCustomWheelComponent::SetSidewaysFrictionProperties(const FWheelFrictionCurve& NewSidewaysFriction)
{
    SidewaysFrictionCurve = NewSidewaysFriction;
    SidewaysFrictionCurve.UpdateArrays();
}


#if WITH_EDITOR
void UCustomWheelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    // Update friction curves if their properties change in editor.
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, ForwardFrictionCurve) || 
        (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, ForwardFrictionCurve)))
    { ForwardFrictionCurve.UpdateArrays(); }
    
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, SidewaysFrictionCurve) ||
        (PropertyChangedEvent.MemberProperty && PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, SidewaysFrictionCurve)))
    { SidewaysFrictionCurve.UpdateArrays(); }

    // Recalculate inertia if mass or radius changes.
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, WheelMass) || PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, WheelRadius))
    {
        WheelInertia = 0.5f * WheelMass * FMath::Square(WheelRadius);
        if (WheelInertia < KINDA_SMALL_NUMBER) { WheelInertia = 0.1f; } // Ensure minimum inertia.
    }

    // Validate slope friction angles.
    if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, FrictionReductionStartAngle) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, FrictionReductionMaxAngle))
    {
        FrictionReductionMaxAngle = FMath::Max(FrictionReductionMaxAngle, FrictionReductionStartAngle);
        FrictionReductionStartAngle = FMath::Min(FrictionReductionStartAngle, FrictionReductionMaxAngle); // Ensure start <= max.
    }
     if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, MinStiffnessMultiplierAtMaxAngle))
    {
        MinStiffnessMultiplierAtMaxAngle = FMath::Clamp(MinStiffnessMultiplierAtMaxAngle, 0.0f, 1.0f);
    }
}
#endif // WITH_EDITOR

