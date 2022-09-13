// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GBMPS_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GB_MPSHOOTER_API UGBMPS_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement", meta = (ClampMin = "1.5", ClampMax = "10.0"))
	float RunModifier = 1.5f;

public:
	virtual float GetMaxSpeed() const override;
};
