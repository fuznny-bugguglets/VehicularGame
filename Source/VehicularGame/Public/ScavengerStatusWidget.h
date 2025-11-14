// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScavengerStatusWidget.generated.h"

enum class EPassiveType : uint8;
/**
 * 
 */
UCLASS()
class VEHICULARGAME_API UScavengerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetCrewAlive();

	UFUNCTION(BlueprintImplementableEvent)
	void SetCrewScavenging();

	UFUNCTION(BlueprintImplementableEvent)
	void SetCrewDead();

	UFUNCTION(BlueprintImplementableEvent)
	void SetPassive(EPassiveType Passive);

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UTexture* DamageUp = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UTexture* FireRate = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UTexture* GatheringSpeed = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UTexture* MagSize = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UTexture* MoveSpeed = nullptr;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess))
	UTexture* ReloadSpeed = nullptr;

	
	
};
