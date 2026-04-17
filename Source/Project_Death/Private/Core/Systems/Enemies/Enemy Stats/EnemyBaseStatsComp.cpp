// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"

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

	if (EnemyCurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Died!!"))
	}
	OnHealthChanged.Broadcast(EnemyCurrentHealth, EnemyMaxHealth);
}

