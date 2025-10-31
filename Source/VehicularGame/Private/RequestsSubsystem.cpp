// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestsSubsystem.h"

#include "InventorySubsystem.h"
#include "Requests.h"
#include "VehicularGameInstance.h"
#include "VehicularSaveGame.h"

void URequestsSubsystem::AddEnemyKill()
{
	LifetimeEnemyKills++;
	CheckKillRequests();
}

void URequestsSubsystem::CheckKillRequests()
{
	for (FRequest Request : URequestsManager::GetRequests())
	{
		//check if it is a kill request
		if (Request.RequestType == ERequestType::KillRequest)
		{
			//check if the request has been achieved
			if (LifetimeEnemyKills >= Request.Count)
			{
				RequestAchieved(Request);
			}
		}
	}
}

void URequestsSubsystem::RequestAchieved(const FRequest& Request)
{
	RequestAchieved(URequestsManager::GetIndexFromRequest(Request));
}


void URequestsSubsystem::RequestAchieved(uint8 RequestID)
{
	//is it in the map?
	if (RequestAchievementStatus.Find(RequestID))
	{
		RequestAchievementStatus[RequestID] = true;
	}
	else
	{
		RequestAchievementStatus.Emplace(RequestID, true);
	}
	
	
}

bool URequestsSubsystem::GetRequestAchievementStatus(uint8 RequestID)
{
	//is it in the map?
	if (!RequestAchievementStatus.Find(RequestID))
	{
		//if it doesn't exist, it is false
		RequestAchievementStatus.Emplace(RequestID, false);
	}

	return RequestAchievementStatus[RequestID];
}

bool URequestsSubsystem::GetRequestRedeemedStatus(uint8 RequestID)
{
	//is it in the map?
	if (!RequestRedeemedStatus.Find(RequestID))
	{
		//if it doesn't exist, it is false
		RequestRedeemedStatus.Emplace(RequestID, false);
	}

	return RequestRedeemedStatus[RequestID];
}

int32 URequestsSubsystem::GetEnemiesKilled() const
{
	return LifetimeEnemyKills;
}

void URequestsSubsystem::RequestRedeemed(uint8 RequestID)
{
	//is it in the map?
	if (RequestRedeemedStatus.Find(RequestID))
	{
		//already exists, set it
		RequestRedeemedStatus[RequestID] = true;
	}
	else
	{
		//if it doesn't exist, make it
		RequestRedeemedStatus.Emplace(RequestID, true);
	}

	//pay the reward
	GetGameInstance()->GetSubsystem<UInventorySubsystem>()->AddMoney(URequestsManager::GetRequestFromIndex(RequestID).MoneyReward);
	
}

void URequestsSubsystem::LoadSaveData()
{
	//get save data
	UVehicularGameInstance* VGameInstance = Cast<UVehicularGameInstance>(GetGameInstance());
	if (VGameInstance)
	{
		if (VGameInstance->GetSaveGameObject())
		{
			RequestAchievementStatus = VGameInstance->GetSaveGameObject()->RequestAchievementStatus;
			RequestRedeemedStatus = VGameInstance->GetSaveGameObject()->RequestRedeemedStatus;
			LifetimeEnemyKills = VGameInstance->GetSaveGameObject()->LifetimeEnemyKills;
		}
	}
	
}


const TMap<uint8, bool>& URequestsSubsystem::GetRequestAchievementStatusMap() const
{
	return RequestAchievementStatus;
}

const TMap<uint8, bool>& URequestsSubsystem::GetRequestRedeemedStatusMap() const
{
	return RequestRedeemedStatus;
}

