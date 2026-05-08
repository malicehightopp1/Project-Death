// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Enemies/Enemy Ui/EnemyUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
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
	HideHealthBar(); //by default off
}
#pragma region Health bar visability 
void UEnemyUI::ShowHealthBar()
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	if(EnemyLevelText)
	{
		EnemyLevelText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UEnemyUI::HideHealthBar()
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
	if(EnemyLevelText)
	{
		EnemyLevelText->SetVisibility(ESlateVisibility::Hidden);
	}
}

#pragma endregion
#pragma region Initilizing UI
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

	UpdateLevelDisplay(enemystats->EnemyLevel);
	OnHealthChanged(enemystats->EnemyCurrentHealth, enemystats->EnemyMaxHealth);
	HideHealthBar(); //hide until enemy hit
}
#pragma endregion
#pragma region Health management
void UEnemyUI::OnHealthChanged(float NewHealth, float MaxHealth)
{
	if (!EnemyHealthBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Bar is NULL"))
	}
	EnemyHealthBar->SetPercent(NewHealth / MaxHealth);
	ShowHealthBar(); //show on damage

	GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &UEnemyUI::HideHealthBar, HideDelay, false);
}
#pragma endregion
#pragma region enemy Level
void UEnemyUI::UpdateLevelDisplay(int32 Level)
{
	if (!EnemyLevelText) return;
	EnemyLevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv: %d"), Level)));
}
#pragma endregion
