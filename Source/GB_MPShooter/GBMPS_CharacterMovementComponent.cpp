// Fill out your copyright notice in the Description page of Project Settings.


#include "GBMPS_CharacterMovementComponent.h"

#include "GBMPS_Warrior.h"

float UGBMPS_CharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const AGBMPS_Warrior* Player = Cast<AGBMPS_Warrior>(GetPawnOwner());
	return Player && Player->IsRunning() ? RunModifier * MaxSpeed : MaxSpeed;
}
