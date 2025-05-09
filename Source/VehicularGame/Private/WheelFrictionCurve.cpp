#include "WheelFrictionCurve.h"

FWheelFrictionCurve::FWheelFrictionCurve()
{
	ExtremumSlip = 0.4f;
	ExtremumValue = 1.0f;
	AsymptoteSlip = 0.8f;
	AsymptoteValue = 0.8f;
	Stiffness = 1.0f;

	// Precompute arrays on construction.
	UpdateArrays();
}

// Cubic Hermite spline interpolation.
FVector2D FWheelFrictionCurve::Hermite(float t, FVector2D p0, FVector2D p1, FVector2D m0, FVector2D m1)
{
	const float t2 = t * t;
	const float t3 = t2 * t;

	return
		p0 * (2.0f * t3 - 3.0f * t2 + 1.0f) +
		m0 * (t3 - 2.0f * t2 + t) +
		p1 * (-2.0f * t3 + 3.0f * t2) +
		m1 * (t3 - t2);
}

// Re-calculates the internal curve points.
void FWheelFrictionCurve::UpdateArrays()
{
	ExtremePoints.SetNum(CurveArraySize);
	AsymptotePoints.SetNum(CurveArraySize);

	// Ensure slips are valid to prevent calculation errors.
	ExtremumSlip = FMath::Max(ExtremumSlip, KINDA_SMALL_NUMBER); // Prevent division by zero.
	AsymptoteSlip = FMath::Max(AsymptoteSlip, ExtremumSlip + KINDA_SMALL_NUMBER); // Asymptote slip must be greater than extremum slip.

	const FVector2D ZeroVec(0.0f, 0.0f);
	const FVector2D ExtremumPoint(ExtremumSlip, ExtremumValue);
	const FVector2D AsymptotePoint(AsymptoteSlip, AsymptoteValue);

	// Define tangents for Hermite spline.
	// Tangent at the start (0,0) for the first segment.
	const FVector2D M0_Ext = FVector2D(ExtremumSlip * 0.5f + 1.0f, 0.0f);
	// Tangent at the end of the first segment and start of the second segment.
	const FVector2D M1_Ext_M0_Asy = FVector2D((AsymptoteSlip - ExtremumSlip) * 0.5f + 1.0f, 0.0f);
	// Tangent at the end of the second segment.
	const FVector2D M1_Asy = M1_Ext_M0_Asy;


	for (int i = 0; i < CurveArraySize; ++i)
	{
		const float t = static_cast<float>(i) / static_cast<float>(CurveArraySize - 1); // Interpolation parameter from 0 to 1.

		// Extremum Segment (from origin to ExtremumPoint).
		ExtremePoints[i].TValue = t;
		ExtremePoints[i].SlipForcePoint = Hermite(t, ZeroVec, ExtremumPoint, ZeroVec, M0_Ext);


		// Asymptote Segment (from ExtremumPoint to AsymptotePoint).
		AsymptotePoints[i].TValue = t;
		AsymptotePoints[i].SlipForcePoint = Hermite(
			t,
			ExtremumPoint,   // P0
			AsymptotePoint,  // P1
			M1_Ext_M0_Asy,   // M0 (Tangent at P0)
			M1_Asy           // M1 (Tangent at P1)
		);
	}
	bArraysUpdated = true;
}

// Internal evaluation using precomputed points.
float FWheelFrictionCurve::EvaluateInternal(float slip, const TArray<FWheelFrictionCurvePoint>& curvePoints) const
{
	if (curvePoints.Num() < 2) return 0.0f; // Curve requires at least two points.

	int top = curvePoints.Num() - 1;
	int bottom = 0;
	int index = bottom + (top - bottom) / 2; // Midpoint for binary search.

	// Check bounds: if slip is outside the precomputed range, return boundary values.
	if (slip <= curvePoints[bottom].SlipForcePoint.X) return curvePoints[bottom].SlipForcePoint.Y;
	if (slip >= curvePoints[top].SlipForcePoint.X) return curvePoints[top].SlipForcePoint.Y;

	// Binary search for the interval containing the slip value.
	while (top - bottom > 1)
	{
		index = bottom + (top - bottom) / 2; // Recalculate midpoint.
		if (curvePoints[index].SlipForcePoint.X <= slip)
		{
			bottom = index;
		}
		else // curvePoints[index].SlipForcePoint.X > slip
		{
			top = index;
		}
	}

	// Slip is between curvePoints[bottom] and curvePoints[top].
	const float slip1 = curvePoints[bottom].SlipForcePoint.X;
	const float slip2 = curvePoints[top].SlipForcePoint.X;
	const float force1 = curvePoints[bottom].SlipForcePoint.Y;
	const float force2 = curvePoints[top].SlipForcePoint.Y;

	// Linear interpolation between the two points.
	// Avoid division by zero if slip points are identical.
	if (FMath::IsNearlyEqual(slip2, slip1))
	{
		return force1; // Slips are equal, return either force.
	}

	const float slipFraction = (slip - slip1) / (slip2 - slip1);
	return FMath::Lerp(force1, force2, slipFraction);
}

// Evaluates the friction curve at a given slip value.
float FWheelFrictionCurve::Evaluate(float slip) const
{
	// Ensure arrays are generated.
	if (!bArraysUpdated)
	{
		UE_LOG(LogTemp, Warning, TEXT("FWheelFrictionCurve::Evaluate called before UpdateArrays! Returning 0."));
		return 0.0f;
	}

	// Slip value must be positive for internal evaluation.
	slip = FMath::Abs(slip);

	if (slip < ExtremumSlip)
	{
		return EvaluateInternal(slip, ExtremePoints) * Stiffness;
	}
	else if (slip < AsymptoteSlip)
	{
		return EvaluateInternal(slip, AsymptotePoints) * Stiffness;
	}
	else // Slip >= AsymptoteSlip
	{
		return AsymptoteValue * Stiffness;
	}
}

#if WITH_EDITOR
void FWheelFrictionCurve::PostEditChange()
{
	UpdateArrays(); // Recompute curve when changed in editor.
}
#endif