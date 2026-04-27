// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/PlayerUI/PlayerWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBoxSlot.h"
#include "Core/Systems/Player/Base/PlayerCurrency/CurrencyManager.h"
#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"

UPlayerWidget::UPlayerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //Constructor 
{

}

void UPlayerWidget::NativeConstruct() //Begin play
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	if (PlayerCursorImage) // setting the padding for 
	{
		if (UVerticalBoxSlot* ImageSlot = Cast<UVerticalBoxSlot>(PlayerCursorImage->Slot))
		{
			FMargin NewPadding(10, 10, 10, 10);
			UE_LOG(LogTemp, Warning, TEXT("Set new padding"));
			ImageSlot->SetPadding(NewPadding);
		}
	}
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		APawn* Pawn = GetOwningPlayerPawn();
		UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), Pawn ? *Pawn->GetName() : TEXT("NULL"));
		if (!Pawn) return;

		UCharacterStatsComp* StatsComp = Pawn->FindComponentByClass<UCharacterStatsComp>();
		if (!StatsComp) return;
		
		UCurrencyManager* CurrencyManager = Pawn->FindComponentByClass<UCurrencyManager>();
		if (!CurrencyManager) return;
		
		CurrencyManager->OnCurrencyChanged.AddDynamic(this, &UPlayerWidget::OnCurrencyChange);
		
		StatsComp->OnLevelChange.AddDynamic(this, &UPlayerWidget::OnLevelChanged);
		StatsComp->OnXpChanged.AddDynamic(this, &UPlayerWidget::OnXpChanged);
		StatsComp->OnStatPointsChanged.AddDynamic(this, &UPlayerWidget::OnStatPointsChanged);
		StatsComp->OnHealthChanged.AddDynamic(this, &UPlayerWidget::OnHealthChanged);
		StatsComp->OnStaminaChanged.AddDynamic(this, &UPlayerWidget::OnStaminaChanged);
		StatsComp->OnFPChanged.AddDynamic(this, &UPlayerWidget::OnManaChange);
		
		OnLevelChanged(StatsComp->CharacterLevel);
		OnXpChanged(StatsComp->CurrentXp, StatsComp->MaxXp);
		OnStatPointsChanged(StatsComp->UnspentStatPoints);
		OnHealthChanged(StatsComp->CurrentHealth, StatsComp->MaxHealth);
		OnStaminaChanged(StatsComp->CurrentStamina, StatsComp->MaxStamina);
		OnManaChange(StatsComp->CurrentMana, StatsComp->MaxMana);
		
		OnCurrencyChange(CurrencyManager->CurrentCurrency);
		
	}, 0.1f, false);
}
void UPlayerWidget::OnLevelChanged(int32 NewCharacterLevel)
{
	if (!PlayerCurrentLevel) return;
	
	FText FormattedLevel = FText::Format(NSLOCTEXT("Level", "LevelKey", "Level: {0}"), 
		FText::AsNumber(NewCharacterLevel));
	
	PlayerCurrentLevel->SetText(FormattedLevel);
}

void UPlayerWidget::OnXpChanged(float NewXp, float MaxXp)
{
	if (!PlayerXp) return;
	
	FText FormattedXP = FText::Format(NSLOCTEXT("Level", "LevelKey", "{0} Xp / {1} Xp"), 
		FText::AsNumber(NewXp), FText::AsNumber(MaxXp));
	
	PlayerXp->SetText(FormattedXP);
	
	if (!PlayerXpBar) return;
	PlayerXpBar->SetPercent(MaxXp > 0.f ? NewXp / MaxXp : 0.f);
}

void UPlayerWidget::OnStatPointsChanged(int32 NewStatPoints)
{
	if (!PlayerStatPoints) return;
	
	FText FormattedStatPoints = FText::Format(NSLOCTEXT("Level", "LevelKey", "StatPoints: {0}"), 
		FText::AsNumber(NewStatPoints));
	
	PlayerStatPoints->SetText(FormattedStatPoints);
}

void UPlayerWidget::OnHealthChanged(float NewHealth, float MaxHealth)
{
	HealthBar->SetPercent(NewHealth / MaxHealth);
	
	FText FormattedHealth = FText::Format(NSLOCTEXT("Level", "LevelKey", "{0} HP / {1} HP"), 
	FText::AsNumber(NewHealth), FText::AsNumber(MaxHealth));
	
	HealthText->SetText(FormattedHealth);
}

void UPlayerWidget::OnStaminaChanged(float NewStamina, float MaxStamina)
{
	StaminaBar->SetPercent(NewStamina / MaxStamina);
	
	FText FormattedStamina= FText::Format(NSLOCTEXT("Level", "LevelKey", "{0} SP / {1} SP"), 
	FText::AsNumber(NewStamina), FText::AsNumber(MaxStamina));
	
	StaminaText->SetText(FormattedStamina);
}

void UPlayerWidget::OnManaChange(float NewMana, float MaxMana)
{
	ManaBar->SetPercent(NewMana / MaxMana);
		
	FText FormattedMana = FText::Format(NSLOCTEXT("Level", "LevelKey", "{0} MP / {1} MP"), 
	FText::AsNumber(NewMana), FText::AsNumber(MaxMana));
	
	ManaText->SetText(FormattedMana);
}

void UPlayerWidget::OnCurrencyChange(int32 NewCurrency)
{
	if (!CurrencyText) return;
	
	FText FormattedCurrency = FText::Format(NSLOCTEXT("Level", "LevelKey", "{0}"), 
		FText::AsNumber(NewCurrency));
	
	CurrencyText->SetText(FormattedCurrency);
}
