#include "CustomWheelComponent.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "Kismet/KismetMathLibrary.h"

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

	SmoothedForwardFrictionForce = FVector::ZeroVector;
	SmoothedSidewaysFrictionForce = FVector::ZeroVector;
	SmoothedFrictionTorque = 0.0f;
}

void UCustomWheelComponent::SetSteerAngle(float Angle)
{
	// Clamp steer angle to a reasonable range.
	SteerAngle = FMath::Clamp(Angle, -90.0f, 90.0f);
}

// Called when the game starts.
void UCustomWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	// Find visual mesh and owner's physics body.
	TArray<USceneComponent*> Children;
	GetChildrenComponents(false, Children);
	if (Children.Num() > 0) { VisualWheelMesh = Children[0]; }
	AActor* Owner = GetOwner();
	if (Owner) { OwningPrimitiveComponent = Cast<UPrimitiveComponent>(Owner->GetRootComponent()); }

	// Initialize previous frame's state variables.
	PreviousSuspensionCompression = SuspensionCompression;
	PreviousPosition = GetComponentLocation();

	// Ensure friction curves are built.
	ForwardFrictionCurve.UpdateArrays();
	SidewaysFrictionCurve.UpdateArrays();

	// Calculate wheel inertia (I = 0.5 * m * r^2 for a solid cylinder).
	// Torque units are kg*cm^2/s^2, consistent with UE base units (cm).
	WheelInertia = 0.5f * WheelMass * FMath::Square(WheelRadius);
	if (WheelInertia < KINDA_SMALL_NUMBER) // Prevent division by zero.
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomWheelComponent '%s' has near-zero Inertia. Clamping."), *GetName());
		WheelInertia = 0.1f; // Assign a minimum inertia.
	}

	SmoothedForwardFrictionForce = FVector::ZeroVector;
	SmoothedSidewaysFrictionForce = FVector::ZeroVector;
	SmoothedFrictionTorque = 0.0f;
}

float UCustomWheelComponent::GetCurrentSmoothingInterpSpeed() const
{
	if (SmoothingAngVelThreshold <= KINDA_SMALL_NUMBER)
	{
		// Avoid division by zero; use min speed if threshold is zero (no dynamic smoothing).
		return MinSmoothingInterpSpeed;
	}

	// Alpha: 1.0 at zero speed, 0.0 above or at threshold.
	const float Alpha = 1.0f - FMath::Clamp(FMath::Abs(WheelAngularVelocity) / SmoothingAngVelThreshold, 0.0f, 1.0f);

	// Linearly interpolate between min and max smoothing speeds based on alpha.
	return FMath::Lerp(MinSmoothingInterpSpeed, MaxSmoothingInterpSpeed, Alpha);
}


void UCustomWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ensure the owning primitive component is valid.
	if (!OwningPrimitiveComponent)
	{
		AActor* CurrentOwner = GetOwner();
		if (CurrentOwner) { OwningPrimitiveComponent = Cast<UPrimitiveComponent>(CurrentOwner->GetRootComponent()); }
		if (!OwningPrimitiveComponent) { return; } // Cannot proceed without a physics body.
	}

	// Store previous compression for damping calculation.
	PreviousSuspensionCompression = SuspensionCompression;

	// Tick Order:
	// 1. Update suspension state.
	UpdateSuspension(DeltaTime);

	// 2. Calculate tire slips.
	CalculateSlips(DeltaTime);

	// 3. Update wheel rotational physics (angular velocity from torques).
	UpdateWheelPhysics(DeltaTime);

	// Update visual rotation angle based on new angular velocity.
	WheelRotationAngle -= WheelAngularVelocity * DeltaTime;

	// 4. Calculate and apply forces (suspension and tire) to the owner.
	CalculateAndApplyForces(DeltaTime);

	// 5. Update visual mesh position and rotation.
	UpdateWheelVisuals(DeltaTime);

	// 6. Store current position for next frame's slip calculation.
	PreviousPosition = GetComponentLocation();
}

void UCustomWheelComponent::UpdateSuspension(float DeltaTime)
{
	// Calculate start and end points for the suspension raycast in world space.
	const FVector ComponentUp = GetUpVector();
	const FVector StartLocation = GetComponentLocation();
	const float TraceLength = SuspensionMaxLength + WheelRadius;
	const FVector EndLocation = StartLocation - ComponentUp * TraceLength;
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(CustomWheelTrace), false, GetOwner());
	QueryParams.bTraceComplex = true; // Use complex collision for more accurate results.

	bool bHit = GetWorld()->LineTraceSingleByChannel(CurrentHitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	bIsGrounded = bHit;

	if (bIsGrounded) {
		// Calculate suspension compression based on hit distance.
		SuspensionCompression = FMath::Clamp(SuspensionMaxLength - (CurrentHitResult.Distance - WheelRadius), 0.0f, SuspensionMaxLength);
		if (bDrawDebug) { DrawDebugLine(GetWorld(), StartLocation, CurrentHitResult.ImpactPoint, FColor::Green, false, -1.f, 0, 0.5f); DrawDebugSphere(GetWorld(), CurrentHitResult.ImpactPoint, 5.f, 8, FColor::Green, false, -1.f, 0, 0.5f); }
	}
	else {
		// Wheel is not grounded, no compression.
		SuspensionCompression = 0.0f;
		CurrentHitResult.Reset(); // Clear previous hit result.
		if (bDrawDebug) { DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 0.5f); }
	}
}

void UCustomWheelComponent::CalculateSlips(float DeltaTime)
{
	if (!bIsGrounded || DeltaTime <= KINDA_SMALL_NUMBER)
	{
		ForwardSlip = 0.0f; // Reset slips if not grounded or DeltaTime is too small.
		SidewaysSlip = 0.0f;
		return;
	}

	// Calculate linear velocity of the wheel's contact point.
	const FVector CurrentPosition = GetComponentLocation();
	const FVector Velocity = (CurrentPosition - PreviousPosition) / DeltaTime;

	// Calculate wheel's local axes considering steer angle.
	const FQuat ComponentRotation = GetComponentQuat();
	const FVector BaseForward = ComponentRotation.GetForwardVector(); // Component's local forward.
	const FVector BaseRight = ComponentRotation.GetRightVector();     // Component's local right.
	const FVector BaseUp = ComponentRotation.GetUpVector();       // Component's local up.

	// Create rotation for steer angle around the component's Up vector.
	const FQuat SteerRotation = FQuat(BaseUp, FMath::DegreesToRadians(SteerAngle));

	// Calculate effective forward and right vectors after steering.
	const FVector SteeredForward = SteerRotation.RotateVector(BaseForward);
	const FVector SteeredSideways = SteerRotation.RotateVector(-BaseRight); // Sideways is opposite to steered right.

	// Project wheel velocity onto steered forward and sideways directions.
	const float ForwardVelocityComponent = FVector::DotProduct(Velocity, SteeredForward);
	const float SidewaysVelocityComponent = FVector::DotProduct(Velocity, SteeredSideways);

	// Calculate wheel's surface speed due to rotation.
	const float WheelSurfaceSpeedRad = FMath::DegreesToRadians(WheelAngularVelocity);
	const float WheelSurfaceSpeed = WheelSurfaceSpeedRad * WheelRadius;

	// Calculate slips.
	ForwardSlip = -FMath::Sign(ForwardVelocityComponent) * FMath::Abs(ForwardVelocityComponent) + WheelSurfaceSpeed;
	SidewaysSlip = -FMath::Sign(SidewaysVelocityComponent) * FMath::Abs(SidewaysVelocityComponent);

	if (bDrawDebug)
	{
		// Logs for debugging slip values.
		// UE_LOG(LogTemp, Log, TEXT("%s - FwdVel: %.2f, SideVel: %.2f, SurfSpeed: %.2f, FwdSlip: %.2f, SideSlip: %.2f"), *GetName(), ForwardVelocityComponent, SidewaysVelocityComponent, WheelSurfaceSpeed, ForwardSlip, SidewaysSlip);
	}
}

void UCustomWheelComponent::UpdateWheelPhysics(float DeltaTime)
{
	if (DeltaTime <= KINDA_SMALL_NUMBER || WheelInertia <= KINDA_SMALL_NUMBER) return;

	// Apply motor torque.
	float MotorAccelRad = MotorTorqueInput / WheelInertia; // Angular acceleration = Torque / Inertia.
	WheelAngularVelocity += FMath::RadiansToDegrees(MotorAccelRad * DeltaTime); // Update angular velocity.

	// Apply brake torque.
	if (BrakeTorqueInput > KINDA_SMALL_NUMBER) {
		float MaxBrakeDecelRad = BrakeTorqueInput / WheelInertia;
		float DeltaVelBrakeRad = MaxBrakeDecelRad * DeltaTime;
		float CurrentAngVelRad = FMath::DegreesToRadians(WheelAngularVelocity);
		// Ensure brake doesn't reverse wheel direction, only slows it down.
		float ActualDeltaVelBrakeRad = FMath::Min(FMath::Abs(CurrentAngVelRad), DeltaVelBrakeRad);
		WheelAngularVelocity -= FMath::Sign(CurrentAngVelRad) * FMath::RadiansToDegrees(ActualDeltaVelBrakeRad);
	}

	// Calculate and apply smoothed friction torque.
	if (bIsGrounded)
	{
		// Raw friction torque from forward slip.
		const float SignedForwardForce = GetForwardFrictionForce(); // Force from current slip.
		const float RawFrictionTorque = -SignedForwardForce * WheelRadius; // Torque = Force * Radius.

		// Interpolation speed for smoothing.
		const float CurrentInterpSpeed = GetCurrentSmoothingInterpSpeed();

		// Smooth the friction torque.
		SmoothedFrictionTorque = FMath::FInterpTo(
			SmoothedFrictionTorque, // Previous smoothed value.
			RawFrictionTorque,      // Target raw value.
			DeltaTime,
			CurrentInterpSpeed      // Dynamic interpolation speed.
		);

		// Angular acceleration from smoothed friction torque.
		const float SmoothedFrictionAccelRad = SmoothedFrictionTorque / WheelInertia;
		// Apply smoothed friction effect to angular velocity.
		WheelAngularVelocity += FMath::RadiansToDegrees(SmoothedFrictionAccelRad * DeltaTime);
	}
	else
	{
		// Reset smoothed torque if not grounded.
		SmoothedFrictionTorque = 0.0f;
	}
}

void UCustomWheelComponent::CalculateAndApplyForces(float DeltaTime)
{
	if (bIsGrounded && OwningPrimitiveComponent && OwningPrimitiveComponent->IsSimulatingPhysics())
	{
		// Calculate suspension forces.
		const FVector ComponentUp = GetUpVector(); // Suspension force acts along component's up vector.
		const float SpringForceMagnitude = (SuspensionCompression - (SuspensionMaxLength * SuspensionTargetPosition)) * SuspensionForce;
		float DampingForceMagnitude = 0.0f;
		if (DeltaTime > KINDA_SMALL_NUMBER) {
			const float CompressionVelocity = (SuspensionCompression - PreviousSuspensionCompression) / DeltaTime;
			DampingForceMagnitude = CompressionVelocity * SuspensionDamping;
		}
		const FVector SuspensionForceVector = ComponentUp * (SpringForceMagnitude + DampingForceMagnitude);

		// Calculate and smooth tire friction forces.
		// Get steered axes for force application.
		const FQuat ComponentRotation = GetComponentQuat();
		const FVector BaseForward = ComponentRotation.GetForwardVector();
		const FVector BaseRight = ComponentRotation.GetRightVector();
		const FVector BaseUp = ComponentRotation.GetUpVector();
		const FQuat SteerRotation = FQuat(BaseUp, FMath::DegreesToRadians(SteerAngle));
		const FVector SteeredForward = SteerRotation.RotateVector(BaseForward);
		const FVector SteeredRight = SteerRotation.RotateVector(BaseRight);

		// Raw friction forces based on current slip, applied along steered axes.
		const float RawForwardForceMag = GetForwardFrictionForce(); // Magnitude from forward slip.
		const FVector RawForwardForceVector = SteeredForward * RawForwardForceMag; // Applied along steered forward.

		const float RawSidewaysForceMagSigned = FMath::Sign(SidewaysSlip) * SidewaysFrictionCurve.Evaluate(SidewaysSlip);
		const FVector RawSidewaysForceVector = -SteeredRight * RawSidewaysForceMagSigned; // Applied along negative steered right.

		// Interpolation speed for smoothing.
		const float CurrentInterpSpeed = GetCurrentSmoothingInterpSpeed();

		// Smooth the friction forces.
		SmoothedForwardFrictionForce = FMath::VInterpTo(SmoothedForwardFrictionForce, RawForwardForceVector, DeltaTime, CurrentInterpSpeed);
		SmoothedSidewaysFrictionForce = FMath::VInterpTo(SmoothedSidewaysFrictionForce, RawSidewaysForceVector, DeltaTime, CurrentInterpSpeed);

		// Combine all forces.
		const FVector TotalForce = SuspensionForceVector + SmoothedForwardFrictionForce + SmoothedSidewaysFrictionForce;

		// Apply total force at the contact point.
		const FVector ApplyLocation = CurrentHitResult.ImpactPoint;
		OwningPrimitiveComponent->AddForceAtLocation(TotalForce, ApplyLocation);

		// Debug drawing for forces.
		if (bDrawDebug) {
			// Draw smoothed forces.
			DrawDebugLine(GetWorld(), ApplyLocation, ApplyLocation + SuspensionForceVector.GetClampedToMaxSize(1000.0f) * 0.01f, FColor::Blue, false, -1.f, 0, 1.0f);
			DrawDebugLine(GetWorld(), ApplyLocation, ApplyLocation + SmoothedForwardFrictionForce.GetClampedToMaxSize(1000.0f) * 0.01f, FColor::Red, false, -1.f, 0, 1.0f);
			DrawDebugLine(GetWorld(), ApplyLocation, ApplyLocation + SmoothedSidewaysFrictionForce.GetClampedToMaxSize(1000.0f) * 0.01f, FColor::Green, false, -1.f, 0, 1.0f);
		}
	}
	else
	{
		// Reset smoothed forces if not grounded.
		SmoothedForwardFrictionForce = FVector::ZeroVector;
		SmoothedSidewaysFrictionForce = FVector::ZeroVector;
	}
}
float UCustomWheelComponent::GetForwardFrictionForce() const
{
	if (!bIsGrounded) return 0.0f;
	// Evaluate curve based on slip and apply correct sign.
	return FMath::Sign(ForwardSlip) * ForwardFrictionCurve.Evaluate(ForwardSlip);
}

void UCustomWheelComponent::UpdateWheelVisuals(float DeltaTime)
{
	if (!VisualWheelMesh) return;

	// Update visual wheel position based on suspension compression.
	const float WheelZOffset = -(SuspensionMaxLength - SuspensionCompression);
	FVector CurrentRelativeLocation = VisualWheelMesh->GetRelativeLocation();
	VisualWheelMesh->SetRelativeLocation(FVector(CurrentRelativeLocation.X, CurrentRelativeLocation.Y, WheelZOffset));

	// Apply combined rotation (steering and spin) using Quaternions.
	// 1. Quaternion for Steering (Yaw around local Z-axis).
	const FQuat SteerQuat = FQuat(FRotator(0.0f, SteerAngle, 0.0f));
	// 2. Quaternion for Spin (Pitch around local Y-axis).
	// Use the accumulated WheelRotationAngle.
	const FQuat SpinQuat = FQuat(FRotator(WheelRotationAngle, 0.0f, 0.0f));
	// 3. Combine rotations: Apply Spin locally first, then Steer.
	const FQuat TargetRelativeQuat = SteerQuat * SpinQuat;
	// 4. Set the final relative rotation of the visual mesh.
	VisualWheelMesh->SetRelativeRotation(TargetRelativeQuat);
}

#if WITH_EDITOR
void UCustomWheelComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	// Update friction curves if their properties are changed in the editor.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, ForwardFrictionCurve) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, ExtremumSlip) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, ExtremumValue) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, AsymptoteSlip) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, AsymptoteValue) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, Stiffness)) { ForwardFrictionCurve.UpdateArrays(); }
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, SidewaysFrictionCurve) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, ExtremumSlip) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, ExtremumValue) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, AsymptoteSlip) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, AsymptoteValue) || PropertyName == GET_MEMBER_NAME_CHECKED(FWheelFrictionCurve, Stiffness)) { SidewaysFrictionCurve.UpdateArrays(); }

	// Recalculate inertia if mass or radius changes.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, WheelMass) || PropertyName == GET_MEMBER_NAME_CHECKED(UCustomWheelComponent, WheelRadius))
	{
		WheelInertia = 0.5f * WheelMass * FMath::Square(WheelRadius);
		if (WheelInertia < KINDA_SMALL_NUMBER) { WheelInertia = 0.1f; } // Ensure minimum inertia.
	}
}
#endif // WITH_EDITOR