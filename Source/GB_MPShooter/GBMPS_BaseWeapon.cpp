// Fill out your copyright notice in the Description page of Project Settings.


#include "GBMPS_BaseWeapon.h"

#include "GBMPS_Projectile.h"

// Sets default values
AGBMPS_BaseWeapon::AGBMPS_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComponent");
	SetRootComponent(WeaponMeshComponent);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGBMPS_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AGBMPS_BaseWeapon::ShootReady_Implementation()
{
	
}

void AGBMPS_BaseWeapon::Shoot_Implementation()
{
	if(!GetWorld()) return;
	auto MuzzleTransform = WeaponMeshComponent->GetSocketTransform("MuzzleFlashSocket");
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.Instigator = GetOwner<APawn>();
	ActorSpawnParameters.Owner = this;
	GetWorld()->SpawnActor<AGBMPS_Projectile>(ProjectileType, MuzzleTransform, ActorSpawnParameters);
}

