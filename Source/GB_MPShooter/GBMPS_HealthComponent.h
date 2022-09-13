// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GBMPS_HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthPercentageChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GB_MPSHOOTER_API UGBMPS_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGBMPS_HealthComponent();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	float GetHealthPercentage() const { return Health / MaxHealth; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return FMath::IsNearlyZero(Health); };
	
	FOnDeath OnDeath;
	FOnHealthPercentageChanged OnHealthPercentageChanged;
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxHealth, meta = (ClampMin = "100.0", ClampMax = "500.0"))
	float MaxHealth = 100.0f;
	
private:
	UPROPERTY(Transient, ReplicatedUsing=OnRep_Health)
	float Health = 0.0f;	

	UPROPERTY()
	class AGBMPS_Warrior* Player;
	
	UFUNCTION(BlueprintCallable)
	void OnRep_Health();

	UFUNCTION(BlueprintCallable)
	void OnRep_MaxHealth();

	UFUNCTION()
	void OnTakeAnyDamageHandler(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
