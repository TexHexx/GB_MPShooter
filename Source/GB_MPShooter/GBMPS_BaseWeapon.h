// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GBMPS_BaseWeapon.generated.h"

UCLASS()
class GB_MPSHOOTER_API AGBMPS_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AGBMPS_BaseWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Params")
	TSubclassOf<class AGBMPS_Projectile> ProjectileType;
	
protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Shoot();

	UFUNCTION(Client, Reliable)
	void ShootReady();
};
