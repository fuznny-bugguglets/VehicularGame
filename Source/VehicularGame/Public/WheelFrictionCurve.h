#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Math/Vector2D.h"
#include "WheelFrictionCurve.generated.h"

// Describes a single point on the friction curve.
USTRUCT(BlueprintType)
struct FWheelFrictionCurvePoint
{
	GENERATED_BODY()

		// Parameter value, not directly editable, calculated.
		UPROPERTY()
		float TValue = 0.0f;

	// Slip (X) and Force (Y) values at this point, not directly editable, calculated.
	UPROPERTY()
		FVector2D SlipForcePoint = FVector2D::ZeroVector;
};


/**
 * Defines a friction curve based on slip.
 */
USTRUCT(BlueprintType)
struct VEHICULARGAME_API FWheelFrictionCurve
{
	GENERATED_BODY()

private:
	// Precomputed curve points for the extremum segment.
	TArray<FWheelFrictionCurvePoint> ExtremePoints;
	// Precomputed curve points for the asymptote segment.
	TArray<FWheelFrictionCurvePoint> AsymptotePoints;

	// Resolution of the precomputed arrays.
	static const int CurveArraySize = 50;

	// Flag indicating if the curve arrays have been computed.
	bool bArraysUpdated = false;

	// Cubic Hermite spline interpolation.
	static FVector2D Hermite(float t, FVector2D p0, FVector2D p1, FVector2D m0, FVector2D m1);

	// Internal evaluation using precomputed points and binary search.
	float EvaluateInternal(float slip, const TArray<FWheelFrictionCurvePoint>& curvePoints) const;


public:
	// Extremum point slip value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Friction Curve", meta = (ToolTip = "Extremum point slip value (X-axis)."))
		float ExtremumSlip = 0.4f;

	// Force at the extremum slip.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Friction Curve", meta = (ToolTip = "Force at the extremum slip (Y-axis)."))
		float ExtremumValue = 1.0f;

	// Asymptote point slip value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Friction Curve", meta = (ToolTip = "Asymptote point slip value (X-axis)."))
		float AsymptoteSlip = 0.8f;

	// Force at the asymptote slip.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Friction Curve", meta = (ToolTip = "Force at the asymptote slip (Y-axis)."))
		float AsymptoteValue = 0.8f;

	// Multiplier affecting overall curve stiffness.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Friction Curve", meta = (ToolTip = "Multiplier affecting overall curve stiffness."))
		float Stiffness = 1.0f;

	FWheelFrictionCurve();

	// Recalculates internal curve points from properties.
	void UpdateArrays();

	// Evaluates the friction curve at a given slip value.
	float Evaluate(float slip) const;

#if WITH_EDITOR
	// Updates arrays when properties change in the editor.
	void PostEditChange();
#endif
};