// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"

#include "Core/Systems/Player/Base/BaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	Attributes.Vitality = 1;
	Attributes.Endurance = 1;
	Attributes.Mind = 1;
	Attributes.Dexterity = 1;
	Attributes.Strength = 1;

	RecalculateDerivedStats();
	
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

float UCharacterStatsComp::GetTotalDamage() const
{
	float StrengthBonus = Attributes.Strength * 2.5f; //the 2.5 is the multipy thay the strength sttribute gives to damage so base is 10 and say the strength level is 10 - 25 damage total damage will be 35
	return BaseDamage + StrengthBonus + EquipmentBonusDamage;
}

void UCharacterStatsComp::ApplyEquipmentBonuses(float BonusHP, float BonusStam, float BonusMana, float BonusDamage)
{
	EquipmentBonusHealth  = BonusHP;
	EquipmentBonusStamina = BonusStam;
	EquipmentBonusMana = BonusMana;
	EquipmentBonusDamage = BonusDamage;

	RecalculateDerivedStats(); //update stats immediatly
}

bool UCharacterStatsComp::SpendStatPoint(FName AttributeName)
{
	if (UnspentStatPoints <= 0) return false;

	if (AttributeName == "Vitality") Attributes.Vitality++;
	else if (AttributeName == "Endurance") Attributes.Endurance++;
	else if (AttributeName == "Mind") Attributes.Mind++;
	else if (AttributeName == "Strength") Attributes.Strength++;
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

	if (const FAttributeStatRow* Row = Lookup(Attributes.Vitality))
	{
		MaxHealth = Row->VitalityHP + EquipmentBonusHealth; //adds the equipment bonus on top of base stats
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	}

	if (const FAttributeStatRow* Row = Lookup(Attributes.Endurance))
	{
		MaxStamina = Row->EnduranceStamina + EquipmentBonusStamina;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.f, MaxStamina);
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}

	if (const FAttributeStatRow* Row = Lookup(Attributes.Mind))
	{
		MaxMana = Row->MindMana + EquipmentBonusMana;
		CurrentMana = FMath::Clamp(CurrentMana, 0.f, MaxMana);
		OnFPChanged.Broadcast(CurrentMana, MaxMana);
	}
}
#pragma region XpAndLeveling 
void UCharacterStatsComp::OnXpChange(float mXpAddAmount)
{
	CurrentXp += mXpAddAmount;
	OnXpPickup.Broadcast(mXpAddAmount, FText::FromString("XP")); //to tell the player how much they got
	
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
	
	//setting stats to max on level up *common rpg practice*
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	CurrentMana = MaxMana;
	
	OnLevelChange.Broadcast(CharacterLevel);
	OnStatPointsChanged.Broadcast(UnspentStatPoints);
	OnXpChanged.Broadcast(CurrentXp, MaxXp);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	OnFPChanged.Broadcast(CurrentMana, MaxMana);
	
	//spawning a leveling affect
	if (LevelUpEffect)
    {
        if (AActor* Owner = GetOwner())
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                LevelUpEffect,
                Owner->GetActorLocation(),
                Owner->GetActorRotation()
            );
        }
    }
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
	if (bIsInvinciable) return;

	CurrentHealth -= mHealthAddAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	if (CurrentHealth <= 0)
	{
		PlayerDeath();
	}
	else
	{
		OnHitReact.Broadcast();
		StartHitStun(); // only stun if still alive
	}
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UCharacterStatsComp::StartHitStun()
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
	Player->bIsAttacking = true;
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	bIsStunned = true;
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	OwnerCharacter->GetCharacterMovement()->DisableMovement();

	GetWorld()->GetTimerManager().ClearTimer(HitStunTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		HitStunTimerHandle,
		this,
		&UCharacterStatsComp::EndHitStun,
		HitStunDuration,
		false
	);
}

void UCharacterStatsComp::EndHitStun()
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(GetOwner());
	Player->bIsAttacking = false;
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	bIsStunned = false;
	if (!bIsPlayerDead)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}
void UCharacterStatsComp::PlayerDeath()
{
	UE_LOG(LogTemp, Warning, TEXT("Player has died!!!"))
	bIsPlayerDead = true;
	OnDeath.Broadcast();
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	}
	if (DeathMontage)
	{
		USkeletalMeshComponent* Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		if (Mesh)
		{
			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(DeathMontage); //play this animation then call the death from an anim notify 
			}
		}
	}
}
#pragma endregion PlayerHealth
#pragma region PlayerStamina
void UCharacterStatsComp::OnStaminaChange(float NewStamina)
{
	CurrentStamina = FMath::Max(CurrentStamina - NewStamina, 0.f);
	RegenDelayTimer = RegenDelay; // reset the delay every time stamina is spent
	StaminaRegen = MaxStamina / 10;
	UE_LOG(LogTemp,Warning,TEXT("StaminaRegen = %f"),StaminaRegen);
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
