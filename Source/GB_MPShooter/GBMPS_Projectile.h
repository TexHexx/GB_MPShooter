// TexHexx Production

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GBMPS_Projectile.generated.h"

UCLASS()
class GB_MPSHOOTER_API AGBMPS_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGBMPS_Projectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UProjectileMovementComponent* MovementComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
