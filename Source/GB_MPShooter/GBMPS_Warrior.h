// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GBMPS_Warrior.generated.h"

UCLASS()
class GB_MPSHOOTER_API AGBMPS_Warrior : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGBMPS_Warrior(const FObjectInitializer& ObjectInitializer);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UGBMPS_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UWidgetComponent* HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapons")
	TSubclassOf<class AGBMPS_BaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animations")
	class UAnimMontage* DeathAnimMontage;
	
private:
	bool bWantsToRun = false;
	bool bIsMovingForward = false;
	UPROPERTY()
	class AGBMPS_BaseWeapon* CurrentWeapon = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void OnStartRunning();
	void OnStopRunning();

	void OnHealthPercentageChanged(float HealthPercent);
	
	void OnDeath();
	
	UFUNCTION(NetMulticast, Reliable)
	void OnDeathClient();

	virtual void Destroyed() override;
public:
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category="Movement")
	float GetMovementDirection() const;

	UFUNCTION(Reliable, Server)
	void SpawnWeapon(TSubclassOf<AGBMPS_BaseWeapon> WeaponToSpawn);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
