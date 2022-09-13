// Fill out your copyright notice in the Description page of Project Settings.


#include "GBMPS_HealthComponent.h"
#include "GBMPS_Warrior.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGBMPS_HealthComponent::UGBMPS_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGBMPS_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UGBMPS_HealthComponent, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGBMPS_HealthComponent, MaxHealth, COND_None, REPNOTIFY_Always);
}


// Called when the game starts
void UGBMPS_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = GetOwner<AGBMPS_Warrior>();
	if(!Player) return;
	if(Player->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		Health = MaxHealth;
		Player->OnTakeAnyDamage.AddDynamic(this, &UGBMPS_HealthComponent::OnTakeAnyDamageHandler);
	}
}

void UGBMPS_HealthComponent::OnRep_Health()
{
	OnHealthPercentageChanged.Broadcast(GetHealthPercentage());
}

void UGBMPS_HealthComponent::OnRep_MaxHealth()
{
	OnHealthPercentageChanged.Broadcast(GetHealthPercentage());
}

void UGBMPS_HealthComponent::OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0.0f || IsDead()) return;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	if(FMath::IsNearlyZero(Health))
		OnDeath.Broadcast();
}

