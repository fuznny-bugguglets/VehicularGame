// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"

class AGSScavenger;

UCLASS()
class VEHICULARGAME_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicle();

	//temp
	UFUNCTION(BlueprintCallable)
	void SetSpeed(float NewSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Loot Getters and Setters
	int32 GetCommonLootCount() const;
	int32 GetUncommonLootCount() const;
	int32 GetRareLootCount() const;

	void ResetAllLoot();
	void SetCommonLootCount(int32 NewValue);
	void SetUncommonLootCount(int32 NewValue);
	void SetRareLootCount(int32 NewValue);
 
private:
	//the engine sound file
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* EngineSound;
	//how loud the engine sound can get
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMinVolume = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMaxVolume = 2.0f;
	//how fast the engine sound can get
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMinPitch = 0.6f;
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float EngineMaxPitch = 3.0f;
	//how fast the car needs to travel for max volume and speed
	UPROPERTY(EditDefaultsOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float SpeedRequiredForMaxEngineSound = 50.0f;

	//reference to the instance of the engine sound
	UPROPERTY()
	UAudioComponent* EngineSoundInstance;

	//how much loot we have
	int32 CommonLootCount = 0;
	int32 UncommonLootCount = 0;
	int32 RareLootCount = 0;

	//reference to the game state
	UPROPERTY()
	AGSScavenger* ScavengerGameState;

	//how fast we are currently travelling
	float Speed = 0.0f;
	
	//sets the speed and pitch of the engine based on speed
	UFUNCTION()
	void SetEngineSoundValues();

	void LogError(const FString& ErrorMessage);
};
