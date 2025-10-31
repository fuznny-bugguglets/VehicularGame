// Fill out your copyright notice in the Description page of Project Settings.


#include "Requests.h"

TArray<FRequest> URequestsManager::Requests;

void URequestsManager::AddRequest(const FRequest& Request)
{
	Requests.Emplace(Request);
}

void URequestsManager::ClearRequests()
{
	Requests.Empty();
}

const TArray<FRequest>& URequestsManager::GetRequests()
{
	return Requests;
}

FRequest& URequestsManager::GetRequestFromIndex(const uint8 Index)
{
	return Requests[Index];
}


uint8 URequestsManager::GetIndexFromRequest(const FRequest& Request)
{
	int32 OutIndex = 255;
	Requests.Find(Request, OutIndex);

	return OutIndex;
}
