// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

#include "GSScavenger.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AVehicle::LogError(const FString& ErrorMessage)
{
	//if we have the engine pointer, we print to the screen
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ErrorMessage);
	}
	//otherwise, we print to the log
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorMessage);
	}
}

// Sets default values
AVehicle::AVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
	//get the game state
	AGameStateBase* GameStateBase = GetWorld()->GetGameState();
	if(GameStateBase == nullptr)
	{
		LogError(TEXT("Failed to get the game state base in vehicle"));
		return;
	}

	//get the scavenger game state
	ScavengerGameState = Cast<AGSScavenger>(GameStateBase);
	if(ScavengerGameState == nullptr)
	{
		LogError(TEXT("Failed to get the Scavenger Game State from Vehicle"));
		return;
	}

	//creates the sound
	if(EngineSound == nullptr)
	{
		LogError(TEXT("No Engine Sound set in Vehicle"));
		return;
	}
	EngineSoundInstance = UGameplayStatics::CreateSound2D(this, EngineSound);

	//sets the volume of the sound
	SetEngineSoundValues();

	//begins playing
	EngineSoundInstance->Play();
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Loot
int32 AVehicle::GetCommonLootCount() const
{
	return CommonLootCount;
}
int32 AVehicle::GetUncommonLootCount() const
{
	return UncommonLootCount;
}
int32 AVehicle::GetRareLootCount() const
{
	return RareLootCount;
}

void AVehicle::ResetAllLoot()
{
	SetCommonLootCount(0);
	SetUncommonLootCount(0);
	SetRareLootCount(0);
} 
void AVehicle::SetCommonLootCount(int32 NewValue)
{
	CommonLootCount = NewValue;
}
void AVehicle::SetUncommonLootCount(int32 NewValue)
{
	UncommonLootCount = NewValue;
}
void AVehicle::SetRareLootCount(int32 NewValue)
{
	RareLootCount = NewValue;
}

//sets the speed and pitch of the engine based on speed
void AVehicle::SetEngineSoundValues()
{
	//check that the sound instance is in the world and kicking
	if(EngineSoundInstance == nullptr)
	{
		LogError("Trying to set the parameters of the engine sound instance when it doesnt exist in Vehicle");
		return;
	}

	//set new volume
	float NewVolume = (((EngineMaxVolume - EngineMinVolume) / SpeedRequiredForMaxEngineSound) * (Speed)) + EngineMinVolume;
	EngineSoundInstance->SetVolumeMultiplier(NewVolume);

	//set new pitch
	float NewPitch = (((EngineMaxPitch - EngineMinPitch) / SpeedRequiredForMaxEngineSound) * (Speed)) + EngineMinPitch;
	EngineSoundInstance->SetPitchMultiplier(NewPitch);
}

//temp
void AVehicle::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
	SetEngineSoundValues();
}





