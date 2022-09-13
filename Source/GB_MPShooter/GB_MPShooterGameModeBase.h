// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GB_MPShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GB_MPSHOOTER_API AGB_MPShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	float RespawnDelay = 0.0f;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Respawn")
	TSubclassOf<class AGBMPS_Warrior> WarriorClass;
	
public:
	void HeroDied(AController* Controller);
	
protected:
	void RespawnHero(AController * Controller);
};
