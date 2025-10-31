// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RequestsSubsystem.generated.h"

struct FRequest;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API URequestsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void AddEnemyKill();
	int32 GetEnemiesKilled() const;

	void RequestAchieved(uint8 RequestID);
	void RequestAchieved(const FRequest& Request);

	void RequestRedeemed(uint8 RequestID);

	bool GetRequestAchievementStatus(uint8 RequestID);
	bool GetRequestRedeemedStatus(uint8 RequestID);

	void LoadSaveData();
	const TMap<uint8, bool>& GetRequestAchievementStatusMap() const;
	const TMap<uint8, bool>& GetRequestRedeemedStatusMap() const;

private:
	void CheckKillRequests();

	//maps request ID's to achievement status
	TMap<uint8, bool> RequestAchievementStatus;

	//maps request ID's to achievement to redeemed status
	TMap<uint8, bool> RequestRedeemedStatus;

	//how many enemies have been killed over the lifetime of this save file
	int32 LifetimeEnemyKills = 0;
	
};
