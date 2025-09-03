// Fill out your copyright notice in the Description page of Project Settings.


#include "Crew.h"

TArray<FCrew> UCrewManager::Crew;

void UCrewManager::AddCrew(const FCrew& NewCrew)
{
	Crew.Emplace(NewCrew);
}

