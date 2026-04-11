// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "Core/Systems/Player/PlayerUI/PlayerStatsPanel.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


UPlayerStatsPanel::UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPlayerStatsPanel::NativeConstruct() //Begin play
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		APawn* Pawn = GetOwningPlayerPawn();
		UE_LOG(LogTemp, Warning, TEXT("Pawn: %s"), Pawn ? *Pawn->GetName() : TEXT("NULL"));
		if (!Pawn) return;

		UCharacterStatsComp* StatsComp = Pawn->FindComponentByClass<UCharacterStatsComp>();
		if (!StatsComp) return;

		StatsComp->OnLevelChange.AddDynamic(this, &UPlayerStatsPanel::OnLevelChanged);
		StatsComp->OnXpChanged.AddDynamic(this, &UPlayerStatsPanel::OnXpChanged);
		StatsComp->OnStatPointsChanged.AddDynamic(this, &UPlayerStatsPanel::OnStatPointsChanged);
		
		OnLevelChanged(StatsComp->CharacterLevel);
		OnXpChanged(StatsComp->CurrentXp, StatsComp->MaxXp);
		OnStatPointsChanged(StatsComp->UnspentStatPoints);
	}, 0.1f, false);
}
void UPlayerStatsPanel::OnLevelChanged(int32 NewCharacterLevel)
{
	if (!PlayerCurrentLevel) return;
	
	FText FormattedLevel = FText::Format(NSLOCTEXT("Level", "LevelKey", "Level: {0}"), 
		FText::AsNumber(NewCharacterLevel));
	
	PlayerCurrentLevel->SetText(FormattedLevel);
}

void UPlayerStatsPanel::OnXpChanged(float NewXp, float MaxXp)
{
	if (!PlayerXp) return;
	
	FText FormattedXP = FText::Format(NSLOCTEXT("Level", "LevelKey", "{0} Xp / {1} Xp"), 
		FText::AsNumber(NewXp), FText::AsNumber(MaxXp));
	
	PlayerXp->SetText(FormattedXP);
	
	if (!PlayerXpBar) return;
	PlayerXpBar->SetPercent(MaxXp > 0.f ? NewXp / MaxXp : 0.f);
}

void UPlayerStatsPanel::OnStatPointsChanged(int32 NewStatPoints)
{
	if (!PlayerStatPoints) return;
	
	FText FormattedStatPoints = FText::Format(NSLOCTEXT("Level", "LevelKey", "StatPoints: {0}"), 
		FText::AsNumber(NewStatPoints));
	
	PlayerStatPoints->SetText(FormattedStatPoints);
}

