// Copyright Epic Games, Inc. All Rights Reserved.


#include "GB_MPShooterGameModeBase.h"

#include "GBMPS_Warrior.h"
#include "GameFramework/SpectatorPawn.h"

void AGB_MPShooterGameModeBase::HeroDied(AController* Controller)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(SpectatorClass, Controller->GetPawn()->GetActorTransform(), SpawnParameters);
	WarriorClass = Controller->GetPawn()->GetClass();
	Controller->UnPossess();
	Controller->Possess(SpectatorPawn);

	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;
	
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateUObject(this, &AGB_MPShooterGameModeBase::RespawnHero, Controller), RespawnDelay, false);
}

void AGB_MPShooterGameModeBase::RespawnHero(AController* Controller)
{
	if (Controller->IsPlayerController() && WarriorClass)
	{
		// Respawn player hero
		AActor* PlayerStart = FindPlayerStart(Controller);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AGBMPS_Warrior* Hero = GetWorld()->SpawnActor<AGBMPS_Warrior>(WarriorClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation(), SpawnParameters);

		APawn* OldSpectatorPawn = Controller->GetPawn();
		Controller->UnPossess();
		OldSpectatorPawn->Destroy();
		Controller->Possess(Hero);
	}
}
