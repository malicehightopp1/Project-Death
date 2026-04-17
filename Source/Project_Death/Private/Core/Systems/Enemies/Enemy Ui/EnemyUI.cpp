// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Ui/EnemyUI.h"

#include "Components/ProgressBar.h"
#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"

void UEnemyUI::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		AActor* Owner = GetOwningPlayerPawn();
		UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), Owner ? *Owner->GetName() : TEXT("NULL"));
		if (!Owner) return;

		UEnemyBaseStatsComp* EnemyStats = Owner->FindComponentByClass<UEnemyBaseStatsComp>();
		if (!EnemyStats) return;

		EnemyStats->OnHealthChanged.AddDynamic(this, &UEnemyUI::OnHealthChanged);

		OnHealthChanged(EnemyStats->EnemyCurrentHealth, EnemyStats->EnemyMaxHealth);
		
	}, 0.1f, false);
}

void UEnemyUI::OnHealthChanged(float NewHealth, float MaxHealth)
{
	EnemyHealthBar->SetPercent(NewHealth / MaxHealth);
}

