// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"

// Sets default values for this component's properties
UCharacterStatsComp::UCharacterStatsComp()
{
	PrimaryComponentTick.bCanEverTick    = true;
	PrimaryComponentTick.TickInterval   = 0.05f; // 20 Hz
}

// Called when the game starts
void UCharacterStatsComp::BeginPlay()
{
	Super::BeginPlay();
	
	//Setting starting values
	CharacterLevel = 1;
	CurrentXp = 0;
	MaxXp = CalculateXpCostForNextLevel(CharacterLevel);
	UnspentStatPoints = 0;
	
	//Changing UI on start
	OnLevelChange.Broadcast(CharacterLevel);
	OnStatPointsChanged.Broadcast(UnspentStatPoints);
	OnXpChanged.Broadcast(CurrentXp, MaxXp);
}

// Called every frame
void UCharacterStatsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UCharacterStatsComp::OnXpChange(float mXpAddAmount)
{
	CurrentXp += mXpAddAmount;
	
	while (CurrentXp >= MaxXp)
	{
		CurrentXp -= MaxXp;
		PlayerLevelUp();
	}
	OnXpChanged.Broadcast(CurrentXp, MaxXp);
}

void UCharacterStatsComp::PlayerLevelUp()
{
	CharacterLevel++;
	UnspentStatPoints += StatPointsPerLevel;
	
	MaxXp = CalculateXpCostForNextLevel(CharacterLevel);
	
	OnLevelChange.Broadcast(CharacterLevel);
	OnStatPointsChanged.Broadcast(UnspentStatPoints);
	OnXpChanged.Broadcast(CurrentXp, MaxXp);
}

int64 UCharacterStatsComp::CalculateXpCostForNextLevel(int32 Level)
{
	if (Level <= 0) return 100;
	const double L = static_cast<double>(Level);
	const double Cost = 100 + (L * L * 50); // Simple curve: 150, 300, 550, 900...
	return static_cast<int64>(Cost);
}


