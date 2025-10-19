// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class UUpgradeSubsystem;
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

	//initiates the reloading process
	void BeginReload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//the maximum amount of ammo the turret can hold
	UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmoCount = 10;

	//the current amount of ammo the turret has
	UPROPERTY()
	int32 AmmoCount = 0;

	//the time in seconds it takes to reload
	UPROPERTY(EditDefaultsOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	float ReloadTime = 1.0f;

	//whether this turret is currently reloading
	bool bIsReloading = false;

	//how long we have been reloading for
	float ElapsedReloadTime = 0.0f;

	//seconds since we last fired a bullet
	float TimeSinceLastShot = 0.0f;

	//the projectile that should spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BulletBlueprint;

	//the sound that plays when the projectile is spawned
	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USoundBase* BulletSoundFile;

	//reference to the instance of the bullet sound
	UPROPERTY()
	UAudioComponent* BulletSoundInstance = nullptr;

	//bullets per second that is fired
	UPROPERTY(EditDefaultsOnly, Category = "Firing", meta = (AllowPrivateAccess = "true"))
	float FireRate = 4.0f;

	//smaller the number, the more accurate the shots
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

	

	//ticks along the reloading process, only runs while reloading
	void ReloadTick(float DeltaTime);

	FRotator GetRotationWithSpread(const FTransform& InputTransform, const float SpreadAngle) const;

	//pointer to the upgrade subsystem
	UPROPERTY()
	UUpgradeSubsystem* UpgradeSubsystem = nullptr;

	//returns the upgrade subsystem
	UFUNCTION()
	UUpgradeSubsystem* GetUpgradeSubsystem();

	void LogError(const FString& ErrorMessage);

};
