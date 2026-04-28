// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"

#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"

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

