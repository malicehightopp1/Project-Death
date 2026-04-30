// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"

#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UEnemyBaseStatsComp::UEnemyBaseStatsComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UEnemyBaseStatsComp::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyCurrentHealth = EnemyMaxHealth;

	OnHealthChanged.Broadcast(EnemyCurrentHealth, EnemyMaxHealth);
	OnDeathChanged.Broadcast(false);
	bIsEnemyDead = false;
}


// Called every frame
void UEnemyBaseStatsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEnemyBaseStatsComp::EnemyHealthChange(float DamageToTake)
{
	EnemyCurrentHealth -= DamageToTake;

	EnemyCurrentHealth = FMath::Clamp(EnemyCurrentHealth, 0 , EnemyMaxHealth);

	OnHealthChanged.Broadcast(EnemyCurrentHealth, EnemyMaxHealth);
	if (EnemyCurrentHealth <= 0)
	{
		bIsEnemyDead = true;
		EnemyDeath();
		OnDeathChanged.Broadcast(true);
	}
	else
	{
		PlayHitreact();
		StartHitStun();
	}
}

void UEnemyBaseStatsComp::EnemyDeath()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			if(UCharacterStatsComp* playerstats = Pawn->FindComponentByClass<UCharacterStatsComp>())
			{
				if (bIsEnemyDead)
				{
					playerstats->OnXpChange(XpToGive); //giving xp on death
					bIsEnemyDead = false;
					AActor* actor = GetOwner();
					actor->Destroy();
				}
			}
		}
	}
}

void UEnemyBaseStatsComp::StartHitStun()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	// Stop all movement immediately
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	OwnerCharacter->GetCharacterMovement()->DisableMovement();

	// Clear any existing stun timer so rapid hits reset the duration
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

	// Only re-enable if still alive
	if (!bIsEnemyDead)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

void UEnemyBaseStatsComp::PlayHitreact()
{
	if (!hitreactMontage) return;

	// Get the skeletal mesh off the owning actor
	USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance) return;

	if (AnimInstance->Montage_IsPlaying(hitreactMontage)) return;

	AnimInstance->Montage_Play(hitreactMontage);
}

