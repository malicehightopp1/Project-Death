// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Core/Systems/Player/Base/PlayerCurrency/CurrencyManager.h"
#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

class AAIController;

UEnemyBaseStatsComp::UEnemyBaseStatsComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UEnemyBaseStatsComp::BeginPlay()
{
	Super::BeginPlay();

	InitStatsFromLevel(EnemyLevel); //setting stats based on level
	
	EnemyCurrentHealth = EnemyMaxHealth; //setting max health

	//default values and setting values using delegates
	OnHealthChanged.Broadcast(EnemyCurrentHealth, EnemyMaxHealth);
	OnDeathChanged.Broadcast(false);
	bIsEnemyDead = false;
}
void UEnemyBaseStatsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UEnemyBaseStatsComp::InitStatsFromLevel(int32 Level)
{
	EnemyLevel = FMath::Clamp(Level, 1, 100); //clamping for max level

	if (EnemyStatTable)
	{
		FName RowKey = FName(*FString::FromInt(EnemyLevel)); //setting string from the int of enemy level
		const FEnemyStatRow* row = EnemyStatTable->FindRow<FEnemyStatRow>(RowKey, TEXT("InitStatsFromlevel"));

		if (row)
		{
			EnemyMaxHealth = row->EnemyMaxhealth;
			EnemyBaseDamage = row->EnemyBaseDamage;
			XpToGive = row->EnemyXpToGive;
			HitStunDuration = row->HitStunDuration;
			return;
		}
	}

	EnemyMaxHealth = CalculateEnemyHealth(EnemyLevel);
	XpToGive = 50.0f * (EnemyLevel * EnemyLevel * 10.0f);
}

float UEnemyBaseStatsComp::CalculateEnemyHealth(float level)
{
	const double L = static_cast<double>(FMath::Clamp(level, 1, 100));
	return static_cast<float>(100.0f + (L * L * 20.0)); //might change this to y = 100 + (L * 200) does scale as high
}
#pragma region Enemy Health
void UEnemyBaseStatsComp::EnemyHealthChange(float DamageToTake)
{
	if (bIsEnemyDead) return; //dont allow damage if enemy is dead
	
	EnemyCurrentHealth -= DamageToTake;

	EnemyCurrentHealth = FMath::Clamp(EnemyCurrentHealth, 0 , EnemyMaxHealth);

	OnHealthChanged.Broadcast(EnemyCurrentHealth, EnemyMaxHealth);
	if (EnemyCurrentHealth <= 0)
	{
		bIsEnemyDead = true;
		OnDeathChanged.Broadcast(true);
		PlayDeathAndDestroy();
	}
	else //if the enemy doesnt die stun them 
	{
		PlayHitreact();
		StartHitStun();
	}
}
#pragma endregion
#pragma region Enemy Death Management
void UEnemyBaseStatsComp::EnemyDeath()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			if(UCharacterStatsComp* playerstats = Pawn->FindComponentByClass<UCharacterStatsComp>())
			{
				UCurrencyManager* Playercurrency = Pawn->GetComponentByClass<UCurrencyManager>();
				if (bIsEnemyDead)
				{
					playerstats->OnXpChange(XpToGive); //giving xp on death
					Playercurrency->OnCurrencyChange(CoinsToGive);
					
					if (EquippedWeapon && IsValid(EquippedWeapon)) //destroying the enemies weapon along with the mesh when killed
					{
						EquippedWeapon->Destroy();
						EquippedWeapon = nullptr;
					}
					bIsEnemyDead = false;
					AActor* actor = GetOwner();
					actor->Destroy();
				}
			}
		}
	}
}

void UEnemyBaseStatsComp::PlayDeathAndDestroy()
{
	bIsEnemyDead = true;
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		if (AAIController* AICon = Cast<AAIController>(OwnerCharacter->GetController()))
		{
			AICon->StopMovement();
			AICon->BrainComponent->StopLogic("Dead"); // stops the behavior tree entirely
		}
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	}

	if (DeathMontage)
	{
		USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (Mesh)
		{
			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->StopAllMontages(0.1f);
				AnimInstance->Montage_Play(DeathMontage); //play this animation then call the death from an anim notify 
			}
		}
	}
}
#pragma endregion
#pragma region Enemy Stun
void UEnemyBaseStatsComp::StartHitStun()
{
	if (bIsStunned) return;
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;
	if (bIsEnemyDead) return;

	bIsStunned = true;
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	OwnerCharacter->GetCharacterMovement()->DisableMovement();

	GetWorld()->GetTimerManager().ClearTimer(HitStunTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		HitStunTimerHandle,
		this,
		&UEnemyBaseStatsComp::EndHitStun,
		HitStunDuration,
		false
	);
}

void UEnemyBaseStatsComp::EndHitStun()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	bIsStunned = false;
	if (!bIsEnemyDead)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void UEnemyBaseStatsComp::PlayHitreact()
{
	if (bIsEnemyDead) return;
	if (!hitreactMontage) return;

	USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance) return;

	if (AnimInstance->Montage_IsPlaying(hitreactMontage)) return;

	AnimInstance->Montage_Play(hitreactMontage);
	OnHitReact.Broadcast();
}
#pragma endregion