// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Vehicle.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "VehicularGameState.h"

void UPlayerHUD::LogError(const FString& ErrorMessage) const
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

UPlayerHUD::UPlayerHUD(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}



FText UPlayerHUD::FetchSpeedText()
{
	if(!PlayerRef)
	{
		PlayerRef = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(!PlayerRef)
		{
			LogError("failed to get vehicle ref in player hud (you can ignore this in editor mode)");
			return UKismetTextLibrary::Conv_StringToText(TEXT("ERROR"));
		}
	}

	//round the player speed to the closest int
	int32 SpeedAsInt = FMath::RoundToInt32(PlayerRef->GetSpeed());

	return UKismetTextLibrary::Conv_IntToText(FMath::Abs(SpeedAsInt));
}

FText UPlayerHUD::FetchEngineStateText()
{
	if(!PlayerRef)
	{
		PlayerRef = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(!PlayerRef)
		{
			LogError("failed to get vehicle ref in player hud (you can ignore this in editor mode)");
			return UKismetTextLibrary::Conv_StringToText(TEXT("ERROR"));
		}
	}

	switch (PlayerRef->GetEngineState())
	{
	case EEngineState::OFF:
		return UKismetTextLibrary::Conv_StringToText(TEXT("OFF"));

	case EEngineState::CRUISE:
		return UKismetTextLibrary::Conv_StringToText(TEXT("ON"));

	case EEngineState::BOOST1:
		return UKismetTextLibrary::Conv_StringToText(TEXT("TURBO"));

	case EEngineState::BOOST2:
		return UKismetTextLibrary::Conv_StringToText(TEXT("OVERCLOCKED"));
	}
	

	return UKismetTextLibrary::Conv_StringToText(TEXT("ERROR"));
}

FText UPlayerHUD::FetchHandbreakStatusText()
{
	if(!PlayerRef)
	{
		PlayerRef = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(!PlayerRef)
		{
			LogError("failed to get vehicle ref in player hud (you can ignore this in editor mode)");
			return UKismetTextLibrary::Conv_StringToText(TEXT("ERROR"));
		}
	}

	if(PlayerRef->IsHandbrakeActive())
	{
		return UKismetTextLibrary::Conv_StringToText(TEXT("Handbrake On"));
	}
	
	return UKismetTextLibrary::Conv_StringToText(TEXT(""));
}

FText UPlayerHUD::FetchNumericalHPText()
{
	if(!PlayerRef)
	{
		PlayerRef = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(!PlayerRef)
		{
			LogError("failed to get vehicle ref in player hud (you can ignore this in editor mode)");
			return UKismetTextLibrary::Conv_StringToText(TEXT("ERROR"));
		}
	}

	//round the player speed to the closest int
	int32 HealthAsInt = FMath::RoundToInt32(PlayerRef->GetHealth());

	return UKismetTextLibrary::Conv_IntToText(FMath::Abs(HealthAsInt));
}



float UPlayerHUD::FetchHPPercentage()
{
	if(!PlayerRef)
	{
		PlayerRef = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(!PlayerRef)
		{
			LogError("failed to get vehicle ref in player hud (you can ignore this in editor mode)");
			return 0.0f;
		}
	}

	return (PlayerRef->GetHealth() / PlayerRef->GetMaxHealth());
}

float UPlayerHUD::FetchDPMPercentage()
{
	if(!GameStateRef)
	{
		GameStateRef = Cast<AVehicularGameState>(GetWorld()->GetGameState());
		if(!GameStateRef)
		{
			LogError("failed to get game state ref in player hud (you can ignore this in editor mode)");
			return 0.0f;
		}
	}
	
	return GameStateRef->Difficulty / DifficultyMax;
}

