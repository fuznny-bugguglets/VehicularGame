#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "WheelFrictionCurve.h"
#include "CustomWheelComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent, ToolTip = "Simulates physics for a single wheel, handling suspension and forces."))
class VEHICULARGAME_API UCustomWheelComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UCustomWheelComponent();

	// The radius of the wheel in cm.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Suspension", meta = (ToolTip = "The radius of the wheel in cm."))
		float WheelRadius = 35.0f;

	// Max suspension travel distance in cm.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Suspension", meta = (ToolTip = "Max suspension travel distance in cm."))
		float SuspensionMaxLength = 15.0f;

	// The mass of the wheel in kg.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Forces", meta = (ClampMin = "0.01", ToolTip = "The mass of the wheel in kg. Assumed to be a cylinder for inertia calculation."))
		float WheelMass = 15.0f;

	// Suspension spring stiffness.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Suspension", meta = (ClampMin = "0.0", ToolTip = "Suspension spring stiffness."))
		float SuspensionForce = 25000.0f;

	// Suspension damping coefficient.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Suspension", meta = (ClampMin = "0.0", ToolTip = "Suspension damping coefficient."))
		float SuspensionDamping = 2500.0f;

	// Normalized suspension target position.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Suspension", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Normalized suspension target position (0=extended, 1=compressed)."))
		float SuspensionTargetPosition = 0.5f;

	// Properties of tire friction in the forward direction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Friction", meta = (ToolTip = "Properties of tire friction in the forward direction."))
		FWheelFrictionCurve ForwardFrictionCurve;

	// Properties of tire friction in the sideways direction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Friction", meta = (ToolTip = "Properties of tire friction in the sideways direction."))
		FWheelFrictionCurve SidewaysFrictionCurve;

	// Torque applied by the motor to the wheel axle.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Input", meta = (ToolTip = "Torque applied by the motor to the wheel axle (in kg*cm^2/s^2). Can be positive or negative."))
		float MotorTorqueInput = 0.0f;

	// Braking torque applied to the wheel axle.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Input", meta = (ClampMin = "0.0", ToolTip = "Braking torque applied to the wheel axle (in kg*cm^2/s^2). Must be positive."))
		float BrakeTorqueInput = 0.0f;

	// Angular velocity threshold for smoothing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Low Speed Smoothing", meta = (ClampMin = "0.0", ToolTip = "Angular velocity (deg/sec) below which force smoothing starts to increase."))
		float SmoothingAngVelThreshold = 50.0f;

	// Max interpolation speed for smoothing at zero angular velocity.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Low Speed Smoothing", meta = (ClampMin = "0.1", ToolTip = "Max interpolation speed (smoothing amount) applied at zero angular velocity. Lower values = more smoothing."))
		float MaxSmoothingInterpSpeed = 5.0f;

	// Min interpolation speed for smoothing above angular velocity threshold.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wheel Settings|Low Speed Smoothing", meta = (ClampMin = "0.1", ToolTip = "Min interpolation speed (smoothing amount) applied above the angular velocity threshold. Higher values = less smoothing."))
		float MinSmoothingInterpSpeed = 50.0f;

	// If true, draws debug lines for suspension and forces.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (ToolTip = "If true, draws debug lines for suspension and forces."))
		bool bDrawDebug = true;

protected:
	// Called when the game starts.
	virtual void BeginPlay() override;

	// Visual representation of the wheel.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel Internals")
		USceneComponent* VisualWheelMesh = nullptr;

	// Physics body of the owning actor.
	UPROPERTY(Transient)
		UPrimitiveComponent* OwningPrimitiveComponent = nullptr;

	// Stores the result of the suspension raycast.
	FHitResult CurrentHitResult;

	// True if the wheel is currently in contact with a surface.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State")
		bool bIsGrounded = false;

	// Current compression distance of the suspension.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State")
		float SuspensionCompression = 0.0f;

	// Suspension compression from the previous frame.
	float PreviousSuspensionCompression = 0.0f;

	// Current angular velocity of the wheel around its axle (deg/sec).
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State", meta = (ToolTip = "Current angular velocity of the wheel around its axle (deg/sec)."))
		float WheelAngularVelocity = 0.0f;

	// Current total rotation angle of the wheel around its axle in degrees.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State", meta = (ToolTip = "Current total rotation angle of the wheel around its axle, in degrees."))
		float WheelRotationAngle = 0.0f;

	// Calculated forward slip of the tire.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State|Slip")
		float ForwardSlip = 0.0f;

	// Calculated sideways slip of the tire.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State|Slip")
		float SidewaysSlip = 0.0f;

	// Current steering angle of the wheel in degrees.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wheel State", meta = (ToolTip = "Current steering angle of the wheel in degrees."))
		float SteerAngle = 0.0f;

	// Position of the wheel component in the previous frame.
	FVector PreviousPosition = FVector::ZeroVector;

	// Moment of inertia of the wheel around its axle (kg*cm^2).
	float WheelInertia = 0.0f;

	// Smoothed forward friction force vector.
	FVector SmoothedForwardFrictionForce = FVector::ZeroVector;
	// Smoothed sideways friction force vector.
	FVector SmoothedSidewaysFrictionForce = FVector::ZeroVector;
	// Smoothed friction torque acting on the wheel.
	float SmoothedFrictionTorque = 0.0f;


public:
	// Called every frame.
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure, Category = "Wheel State")
		bool GetIsGrounded() const { return bIsGrounded; }

	UFUNCTION(BlueprintPure, Category = "Wheel State")
		float GetSuspensionCompression() const { return SuspensionCompression; }

	UFUNCTION(BlueprintPure, Category = "Wheel State")
		float GetWheelAngularVelocity() const { return WheelAngularVelocity; }

	UFUNCTION(BlueprintPure, Category = "Wheel State|Slip")
		float GetForwardSlip() const { return ForwardSlip; }

	UFUNCTION(BlueprintPure, Category = "Wheel State|Slip")
		float GetSidewaysSlip() const { return SidewaysSlip; }

	UFUNCTION(BlueprintPure, Category = "Wheel State", meta = (ToolTip = "Returns the current steer angle of the wheel in degrees."))
		float GetSteerAngle() const { return SteerAngle; }

	UFUNCTION(BlueprintPure, Category = "Wheel State", meta = (ToolTip = "Returns the current rotation angle of the wheel in degrees."))
		float GetWheelRotationAngle() const { return WheelRotationAngle; }

	UFUNCTION(BlueprintCallable, Category = "Wheel Input", meta = (ToolTip = "Set the motor torque input for this frame."))
		void SetMotorTorque(float Torque) { MotorTorqueInput = Torque; }

	UFUNCTION(BlueprintCallable, Category = "Wheel Input", meta = (ToolTip = "Set the brake torque input for this frame (non-negative)."))
		void SetBrakeTorque(float Torque) { BrakeTorqueInput = FMath::Max(0.0f, Torque); }

	UFUNCTION(BlueprintCallable, Category = "Wheel Input", meta = (ToolTip = "Set the steering angle for this wheel (in degrees)."))
		void SetSteerAngle(float Angle);


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


private:
	// Performs the suspension raycast and updates suspension state.
	void UpdateSuspension(float DeltaTime);

	// Calculates wheel angular velocity based on applied torques and friction.
	void UpdateWheelPhysics(float DeltaTime);

	// Calculates wheel slip based on angular and linear velocities.
	void CalculateSlips(float DeltaTime);

	// Calculates and applies suspension and tire friction forces to the owner.
	void CalculateAndApplyForces(float DeltaTime);

	// Updates the visual representation of the wheel (position and rotation).
	void UpdateWheelVisuals(float DeltaTime);

	// Calculates the raw forward friction force magnitude from the friction curve.
	float GetForwardFrictionForce() const;

	// Calculates the current interpolation speed for force/torque smoothing.
	float GetCurrentSmoothingInterpSpeed() const;
};