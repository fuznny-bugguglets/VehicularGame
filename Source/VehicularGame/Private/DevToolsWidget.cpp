// Fill out your copyright notice in the Description page of Project Settings.


#include "DevToolsWidget.h"

#include "Vehicle.h"
#include "VehicularGameMode.h"
#include "Kismet/GameplayStatics.h"

void UDevToolsWidget::NativeConstruct()
{
	VehicularGameMode = Cast<AVehicularGameMode>(UGameplayStatics::GetGameMode(this));
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

