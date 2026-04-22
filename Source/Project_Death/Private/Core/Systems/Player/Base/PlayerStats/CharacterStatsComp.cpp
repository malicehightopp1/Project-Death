// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"

// Sets default values for this component's properties
UCharacterStatsComp::UCharacterStatsComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f; // 20 Hz
	
	//Default Stamina stats
	RegenDelay = 1.2f;
	StaminaRegen = 2.0f;
	
	//Default Mana Stats
	ManaRegen = 2.0f;
	ManaDelay = 1.2;
}

// Called when the game starts
void UCharacterStatsComp::BeginPlay()
{
	Super::BeginPlay();
	
	//Starting Health Values
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsPlayerDead = false;
	
	//Starting Stamina Values
	MaxStamina = 100.0f;
	CurrentStamina = MaxStamina;
	bIsRegening = false;
	
	//Staring Mana Values
	MaxMana = 100.0f;
	CurrentMana = MaxMana;
	bIsManaRegening = false;
	
	//Starting XP values
	CharacterLevel = 1;
	CurrentXp = 0;
	MaxXp = CalculateXpCostForNextLevel(CharacterLevel);
	UnspentStatPoints = 0;
	
	//Changing UI on start
	OnLevelChange.Broadcast(CharacterLevel);
	OnStatPointsChanged.Broadcast(UnspentStatPoints);
	OnXpChanged.Broadcast(CurrentXp, MaxXp);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	OnFPChanged.Broadcast(CurrentMana, MaxMana);
}
// Called every frame
void UCharacterStatsComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	OnStaminaRegen(DeltaTime); //Regening stamina
	OnManaRegen(DeltaTime);
}

bool UCharacterStatsComp::SpendStatPoint(FName AttributeName)
{
	if (UnspentStatPoints <= 0) return false;

	if (AttributeName == "Vitality")  Attributes.Vitality++;
	else if (AttributeName == "Endurance") Attributes.Endurance++;
	else if (AttributeName == "Mind")      Attributes.Mind++;
	else if (AttributeName == "Strength")  Attributes.Strength++;
	else if (AttributeName == "Dexterity") Attributes.Dexterity++;
	else return false;

	UnspentStatPoints--;
	RecalculateDerivedStats();
	OnStatPointsChanged.Broadcast(UnspentStatPoints);
	OnAttributesChanged.Broadcast(Attributes);
	return true;
}

void UCharacterStatsComp::RecalculateDerivedStats()
{
	if (!AttributeStatTable) return;

	auto Lookup = [&](int32 Level) -> const FAttributeStatRow* 
	{
		//looking up data based on row name 
		return AttributeStatTable->FindRow<FAttributeStatRow>(FName(*FString::FromInt(Level)), TEXT("RecalculateDerivedStats"));
	};

	//Setting new data for upgrading attributes
	if (const FAttributeStatRow* Row = Lookup(Attributes.Vitality))
	{
		MaxHealth = Row->VitalityHP;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	}

	if (const FAttributeStatRow* Row = Lookup(Attributes.Endurance))
	{
		MaxStamina = Row->EnduranceStamina;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}

	if (const FAttributeStatRow* Row = Lookup(Attributes.Mind))
	{
		MaxMana = Row->MindMana;
		CurrentMana = FMath::Clamp(CurrentMana, 0.f, MaxMana);
		OnFPChanged.Broadcast(CurrentMana, MaxMana);
	}
}
#pragma region XpAndLeveling 
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
	//Might switch to 100 + (Level^2 * 100) Feels more appropriate *MORE XP*
	if (Level <= 0) return 100;
	const double L = static_cast<double>(Level); //this used floating point arithmetic (avoiding integer rounding issues)
	const double Cost = 100 + (L * L * 50); // 100 + (Level^2 X 50) Quadratic curve - 100 base cost L * L * 50 is the scaling 
	//Example: Level 1: 100 + (1 * 50) = 150,
	//Level 5: 100 + (25 * 50) = 1350,
	//Level 10: 100 + (100 * 50) = 5100
	return static_cast<int64>(Cost); //returns an integer value  
}
#pragma endregion XpAndLeveling
#pragma region PlayerHealth
void UCharacterStatsComp::OnHealthChange(float mHealthAddAmount)
{
	CurrentHealth -= mHealthAddAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	if (CurrentHealth <= 0)
	{
		PlayerDeath();
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UCharacterStatsComp::PlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Player has died!!!"))
	OnDeath.Broadcast();
}
#pragma endregion PlayerHealth
#pragma region PlayerStamina
void UCharacterStatsComp::OnStaminaChange(float NewStamina)
{
	CurrentStamina = FMath::Max(CurrentStamina - NewStamina, 0.f);
	RegenDelayTimer = RegenDelay; // reset the delay every time stamina is spent
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void UCharacterStatsComp::OnStaminaRegen(float DeltaTime)
{
	if (bIsSprinting)
	{
		CurrentStamina = FMath::Max(CurrentStamina - StaminaDrain * DeltaTime, 0.f);
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

		if (CurrentStamina <= 0.f) StopSprinting(); // force stop when empty
		return;
	}

	if (RegenDelayTimer > 0.f)
	{
		RegenDelayTimer -= DeltaTime;
		return;
	}

	if (CurrentStamina >= MaxStamina) return;

	CurrentStamina = FMath::Min(CurrentStamina + StaminaRegen * DeltaTime, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

void UCharacterStatsComp::StartSprinting()
{
	if (CurrentStamina <= 0.f) return;
	bIsSprinting = true;
	OnSprintChanged.Broadcast(true);
}

void UCharacterStatsComp::StopSprinting()
{
	bIsSprinting = false;
	RegenDelayTimer = RegenDelay;
	OnSprintChanged.Broadcast(false);
}
#pragma endregion PlayerStamina
#pragma region PlayerMana
void UCharacterStatsComp::OnManaChange(float NewMana)
{
	CurrentMana = FMath::Max(CurrentMana - NewMana, 0.f);
	ManaRegenTimer = ManaDelay; // reset the delay every time stamina is spent
	OnFPChanged.Broadcast(CurrentMana, MaxMana);
}

void UCharacterStatsComp::OnManaRegen(float DeltaTime)
{
	if (CurrentMana >= MaxMana) return; //Do nothing if stamina is full
	
	if (ManaRegenTimer > 0.0f)
	{
		ManaRegenTimer -= DeltaTime;
		return;
	}
	CurrentMana = FMath::Min(CurrentMana + ManaRegen * DeltaTime, MaxMana);
	OnFPChanged.Broadcast(CurrentMana, MaxMana);
}
#pragma endregion PlayerMana
