// Fill out your copyright notice in the Description page of Project Settings.


#include "DevToolsWidget.h"

#include "Vehicle.h"
#include "VehicularGameMode.h"
#include "VehicularGameState.h"
#include "Kismet/GameplayStatics.h"

void UDevToolsWidget::NativeConstruct()
{
	VehicularGameMode = Cast<AVehicularGameMode>(UGameplayStatics::GetGameMode(this));
	VehicularGameState = Cast<AVehicularGameState>(UGameplayStatics::GetGameState(this));
	Vehicle = Cast<AVehicle>(UGameplayStatics::GetPlayerPawn(this, 0));
}


void UDevToolsWidget::GiveInfiniteResources()
{
	if (!Vehicle)
	{
		return;
	}

	Vehicle->SetCommonLootCount(99999);
	Vehicle->SetUncommonLootCount(99999);
	Vehicle->SetRareLootCount(99999);
}

void UDevToolsWidget::ForceSpawnEnemyWave()
{
	if (!VehicularGameMode)
	{
		return;
	}

	if (!Vehicle)
	{
		return;
	}

	//spawns the enemies and gets the position of where they were spawned
	FVector SpawnPos = VehicularGameMode->CreateEnemies();
	if (SpawnPos != FVector::Zero())
	{
		DrawDebugLine(GetWorld(), Vehicle->GetActorLocation(), SpawnPos, FColor::Red,
			false, 30.0f, 0, 20.0f);
	}

	
}

void UDevToolsWidget::InfiniteHealth()
{
	if (!Vehicle)
	{
		return;
	}

	Vehicle->SetHealth(99999);
}

void UDevToolsWidget::TeleportToPOI()
{
	if (!Vehicle)
	{
		return;
	}

	TArray<AActor*> POIs;
	UGameplayStatics::GetAllActorsOfClass(this, ARuin::StaticClass(), POIs);
	if (POIs.Num() <= 0)
	{
		return;
	}

	int32 RandIndex = FMath::RandRange(0, (POIs.Num()-1));

	FVector NewLocation = POIs[RandIndex]->GetActorLocation();
	NewLocation += FVector(2000);
	Vehicle->SetActorLocation(NewLocation);
}

void UDevToolsWidget::IncreaseDifficulty()
{
	if (!VehicularGameState)
	{
		return;
	}

	VehicularGameState->IncrementDifficulty();
}

void UDevToolsWidget::DecreaseDifficulty()
{
	if (!VehicularGameState)
	{
		return;
	}

	VehicularGameState->DecrementDifficulty();
}


