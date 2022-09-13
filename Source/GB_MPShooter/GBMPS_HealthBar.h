// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GBMPS_HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class GB_MPSHOOTER_API UGBMPS_HealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
};
