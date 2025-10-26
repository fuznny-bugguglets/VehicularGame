// Fill out your copyright notice in the Description page of Project Settings.


#include "Requests.h"

TArray<FRequest> URequestsManager::Requests;

void URequestsManager::AddRequest(const FRequest& Request)
{
	Requests.Emplace(Request);
}

void URequestsManager::ClearRequests()
{
	
}
