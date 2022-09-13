// Fill out your copyright notice in the Description page of Project Settings.


#include "GBMPS_Warrior.h"

#include "GBMPS_BaseWeapon.h"
#include "GBMPS_CharacterMovementComponent.h"
#include "GBMPS_HealthBar.h"
#include "GBMPS_HealthComponent.h"
#include "GB_MPShooterGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

DECLARE_LOG_CATEGORY_CLASS(Warrior, All, All);
// Sets default values
AGBMPS_Warrior::AGBMPS_Warrior(const FObjectInitializer& ObjectInitializer) :
	Super::ACharacter(ObjectInitializer.SetDefaultSubobjectClass<UGBMPS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarComponent");
	HealthBarComponent->SetWidgetClass(UGBMPS_HealthComponent::StaticClass());
	HealthBarComponent->SetupAttachment(RootComponent);
	//HealthBarComponent->SetOwnerNoSee(true);
	
	HealthComponent = CreateDefaultSubobject<UGBMPS_HealthComponent>("HealthComponent");
	
	bReplicates = true;
}

void AGBMPS_Warrior::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called when the game starts or when spawned
void AGBMPS_Warrior::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->SetIsReplicated(true);
	if(GetLocalRole() == ENetRole::ROLE_Authority)
	{
		SpawnWeapon(WeaponClass);
		HealthComponent->OnDeath.AddUObject(this, &AGBMPS_Warrior::OnDeath);
	}
	else
	{
		HealthComponent->OnHealthPercentageChanged.AddUObject(this, &AGBMPS_Warrior::OnHealthPercentageChanged);
	}
}

void AGBMPS_Warrior::MoveForward(float Axis)
{
	bIsMovingForward = Axis >= 0.0f;
	AddMovementInput(GetActorForwardVector(), Axis);
}

void AGBMPS_Warrior::MoveRight(float Axis)
{
	AddMovementInput(GetActorRightVector(), Axis);
}

void AGBMPS_Warrior::OnStartRunning()
{
	bWantsToRun = true;
}

void AGBMPS_Warrior::OnStopRunning()
{
	bWantsToRun = false;
}

void AGBMPS_Warrior::OnHealthPercentageChanged(float HealthPercent)
{
	if(!HealthBarComponent) return;
	auto HealthBar = Cast<UGBMPS_HealthBar>(HealthBarComponent->GetUserWidgetObject());
	if(HealthBar)
		HealthBar->HealthBar->SetPercent(HealthPercent);
}

void AGBMPS_Warrior::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	AGB_MPShooterGameModeBase* GM = Cast<AGB_MPShooterGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->HeroDied(GetController());
		if(DeathAnimMontage)
		{
			SetLifeSpan(DeathAnimMontage->GetPlayLength() + GM->RespawnDelay);
			OnDeathClient();
		}
		else
		{
			SetLifeSpan(GM->RespawnDelay);
			OnDeathClient();
		}
	}
	else
	{
		Destroy();
	}
}

void AGBMPS_Warrior::OnDeathClient_Implementation()
{
	PlayAnimMontage(DeathAnimMontage);
}

void AGBMPS_Warrior::Destroyed()
{
	if(CurrentWeapon)
		CurrentWeapon->Destroy();
	Super::Destroyed();
}

bool AGBMPS_Warrior::IsRunning() const
{
	return bWantsToRun && bIsMovingForward && !GetVelocity().IsNearlyZero();
}

float AGBMPS_Warrior::GetMovementDirection() const
{
	if(GetVelocity().IsNearlyZero()) return 0.0;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngelBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngelBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AGBMPS_Warrior::SpawnWeapon_Implementation(TSubclassOf<AGBMPS_BaseWeapon> WeaponToSpawn)
{
	if (!GetWorld()) return;
	if (!IsValid(WeaponToSpawn)) return;
	if(CurrentWeapon) CurrentWeapon->Destroy();
	CurrentWeapon = GetWorld()->SpawnActor<AGBMPS_BaseWeapon>(WeaponToSpawn);
	if(CurrentWeapon)
	{
		const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, "WeaponPoint");
	}
}

// Called every frame
void AGBMPS_Warrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetLocalRole() == ENetRole::ROLE_Authority)
	{
		TakeDamage(0.5f, FDamageEvent{}, GetController(), this);
	}
}

// Called to bind functionality to input
void AGBMPS_Warrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveForward", this, &AGBMPS_Warrior::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AGBMPS_Warrior::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AGBMPS_Warrior::AddControllerPitchInput);
	InputComponent->BindAxis("TurnAround", this, &AGBMPS_Warrior::AddControllerYawInput);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AGBMPS_Warrior::Jump);
	InputComponent->BindAction("Run", IE_Pressed, this, &AGBMPS_Warrior::OnStartRunning);
	InputComponent->BindAction("Run", IE_Released, this, &AGBMPS_Warrior::OnStopRunning);
}

