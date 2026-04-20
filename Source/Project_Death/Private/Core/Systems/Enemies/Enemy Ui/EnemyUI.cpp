// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Ui/EnemyUI.h"

#include <gsl/pointers>

#include "Components/ProgressBar.h"
#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"


void UEnemyUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (OwnerActor)
	{
		UEnemyBaseStatsComp* enemystats = OwnerActor->FindComponentByClass<UEnemyBaseStatsComp>();
		if (!enemystats)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Base stats detected"))
		}
			
		enemystats->OnHealthChanged.AddDynamic(this, &UEnemyUI::OnHealthChanged);
		OnHealthChanged(enemystats->EnemyCurrentHealth, enemystats->EnemyMaxHealth);
	}
}
void UEnemyUI::InitwithOwner(AActor* InownerActor)
{
	OwnerActor = InownerActor;
	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerActor is NULL"))
	}
	UEnemyBaseStatsComp* enemystats = OwnerActor->FindComponentByClass<UEnemyBaseStatsComp>();
	if (!enemystats)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Base stats detected"))
	}
	
	enemystats->OnHealthChanged.RemoveDynamic(this, &UEnemyUI::OnHealthChanged);
	enemystats->OnHealthChanged.AddDynamic(this, &UEnemyUI::OnHealthChanged);
	
	OnHealthChanged(enemystats->EnemyCurrentHealth, enemystats->EnemyMaxHealth);
}

void UEnemyUI::OnHealthChanged(float NewHealth, float MaxHealth)
{
	if (!EnemyHealthBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Bar is NULL"))
	}
	EnemyHealthBar->SetPercent(NewHealth / MaxHealth);
}