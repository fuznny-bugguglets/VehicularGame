// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class UTurretRotationComponent;
class UCameraComponent;

UCLASS()
class VEHICULARGAME_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	void InitializeVariables(UCameraComponent* InCameraComponent, APawn* InShootingPawn);
	void FirePressed();
	void FireReleased();
	void FireHeld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//seconds since we last fired a bullet
	float TimeSinceLastShot = 0.0f;

	//the projectile that should spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BulletBlueprint;

	//the sound that plays when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USoundBase* BulletSoundFile;

	//reference to the instance of the bullet sund
	UPROPERTY()
	UAudioComponent* BulletSoundInstance = nullptr;

	//bullets per second that is fired
	UPROPERTY(EditDefaultsOnly, Category = "Firing", meta = (AllowPrivateAccess = "true"))
	float FireRate = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing", meta = (AllowPrivateAccess = "true"))
	float MaxSpreadAngle = 1.0f;

	//whether the bullet fires from the left or right barrel
	bool bFireFromLeftBarrel = true;

	//reference to game state
	UPROPERTY()
	class AVehicularGameState* VehicularGameState;

	//reference to the camera
	UPROPERTY()
	UCameraComponent* VehicleCamera;

	//reference to the shooting pawn
	APawn* ShooterPawn;

	//meshes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretBaseMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretHeadMesh;

	//turret rotator component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTurretRotationComponent* TurretRotationComponent;

	//bullet spawn locations
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BulletSpawnL;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BulletSpawnR;

	float UpgradedFireRate();

	FRotator GetRotationWithSpread(const FTransform& InputTransform, const float SpreadAngle) const;

	void LogError(const FString& ErrorMessage);

};
