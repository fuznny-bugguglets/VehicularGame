#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Targeting defaults
	TargetLineForwardOffset = 1000.0f;
	TargetLineHalfWidth = 2000.0f;
	TargetPointNoiseSpeed = 0.5f;

	// Ground projection defaults
	GroundTraceUpwardOffset = 500.0f;
	GroundTraceDownwardDistance = 5000.0f;

	NoiseTimeAccumulator = FMath::FRand() * 1000.0f;
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		if (GetPawn())
		{
			CurrentTargetLocation = GetPawn()->GetActorLocation();
		}
		return;
	}

	// --- STABILIZED TARGET CALCULATION ---

	// 1. get player's vectors, flattening them to be parallel to the ground.
	FVector PlayerForward = PlayerPawn->GetActorForwardVector();
	PlayerForward.Z = 0.0f;
	PlayerForward.Normalize();

	FVector PlayerRight = PlayerPawn->GetActorRightVector();
	PlayerRight.Z = 0.0f;
	PlayerRight.Normalize();

	// 2. Calculate noise value (-1.0 to 1.0)
	NoiseTimeAccumulator += DeltaTime * TargetPointNoiseSpeed;
	const float HorizontalNoise = FMath::PerlinNoise1D(NoiseTimeAccumulator);

	// 3. calculate the target point on the stabilized horizontal plane.
	const FVector LineCenter = PlayerPawn->GetActorLocation() + (PlayerForward * TargetLineForwardOffset);
	const FVector HorizontalOffset = PlayerRight * HorizontalNoise * TargetLineHalfWidth;
	const FVector HorizontalTargetPoint = LineCenter + HorizontalOffset;
	
	// ground projection
	const FVector TraceStart = HorizontalTargetPoint + FVector(0, 0, GroundTraceUpwardOffset);
	const FVector TraceEnd = HorizontalTargetPoint - FVector(0, 0, GroundTraceDownwardDistance);

	FHitResult HitResult;
	// setup collision query params to ignore the player and itself
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerPawn);
	if(GetPawn())
	{
		CollisionParams.AddIgnoredActor(GetPawn());
	}
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility, // Trace against things that are visible
		CollisionParams
	);

	if (bHit)
	{
		// If it hit the ground, that's the new target.
		CurrentTargetLocation = HitResult.Location;
	}
	else
	{
		// If it doesn't hit anything (like over a cliff), use the calculated horizontal point.
		CurrentTargetLocation = HorizontalTargetPoint;
	}
}

FVector AEnemyAIController::GetMovingTargetLocation() const
{
	return CurrentTargetLocation;
}