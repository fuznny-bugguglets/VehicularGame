// Fill out your copyright notice in the Description page of Project Settings.


#include "Crew.h"

TArray<FCrew> UCrewManager::Crew;

void UCrewManager::AddCrew(const FCrew& NewCrew)
{
	Crew.Emplace(NewCrew);
}

FCrew& UCrewManager::GetCrewFromIndex(const uint8 Index)
{
	return Crew[Index];
}

uint8 UCrewManager::GetIndexFromCrew(const FCrew& InCrew)
{
	int32 OutIndex = 255;
	Crew.Find(InCrew, OutIndex);

	return OutIndex;
}

