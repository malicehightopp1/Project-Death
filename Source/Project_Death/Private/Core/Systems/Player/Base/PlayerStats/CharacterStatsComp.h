// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComp.generated.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Structs --------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FAttributeStatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) int32 AttributeLevel    = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float VitalityHP        = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float EnduranceStamina  = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float MindMana          = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float StrengthDamage    = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) float DexteritySpeed    = 0.f;
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99)) int32 Vitality  = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99)) int32 Endurance = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99)) int32 Mind      = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99)) int32 Strength  = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin=1, ClampMax=99)) int32 Dexterity = 1;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// delegates-------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFPChanged, float, NewFP, float, MaxFP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnCharacterLevelUp, int32, NewCharLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnStatPointsChanged, int32, RemainingPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXpChanged, float, NewXP, float, MaxXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSprintChanged, bool, bSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributesChanged, FCharacterAttributes, NewAttributes);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class UCharacterStatsComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Attributes  ----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | Attributes") UDataTable* AttributeStatTable;

	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | Attributes") FCharacterAttributes Attributes;

	UFUNCTION(BlueprintCallable, Category = "Character Stats | Attributes") bool SpendStatPoint(FName AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Character Stats | Attributes") void RecalculateDerivedStats();

	UPROPERTY(BlueprintAssignable, Category = "Character Stats | Attributes") FOnAttributesChanged OnAttributesChanged;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// XP -------------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//XP functions
	UFUNCTION(BlueprintCallable, Category = "Character Stats | Level") void OnXpChange(float mXpAddAmount);
	
	//Xp Variables
	UPROPERTY(BlueprintReadOnly, Category = "Characeter Stats | Level") int32 CharacterLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Characeter Stats | Level") int32 UnspentStatPoints = 0; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Stats | Level",meta = (ClampMin = 1, ClampMax = 10)) int32 StatPointsPerLevel = 1; //stat points the player gets per level up

	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | Level") int64 CurrentXp= 0;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | Level") int64 MaxXp = 0;
	
	//XP Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stats | Level") FOnCharacterLevelUp OnLevelChange; 
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stats | Level") FOnStatPointsChanged OnStatPointsChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stats | Level") FOnXpChanged OnXpChanged;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Health----------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Health Functions
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterHealth") void OnHealthChange(float mHealthAddAmount);
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterHealth") void PlayerDeath();
	
	//Health Variables
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterHealth") float CurrentHealth = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterHealth") float MaxHealth = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterHealth") bool bIsPlayerDead = false;
	
	//Health Delegates 
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Character Stats | CharacterHealth") FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Character Stats | CharacterHealth") FOnDeath OnDeath;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Stamina---------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Stamina Functions
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterStamina") void OnStaminaChange(float NewStamina);
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterStamina") void OnStaminaRegen(float DeltaTime);
	UFUNCTION(BlueprintCallable) void StartSprinting();
	UFUNCTION(BlueprintCallable) void StopSprinting();
	
	//Stamina Variables
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterStamina") float CurrentStamina = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterStamina") float MaxStamina = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | CharacterStamina") float StaminaRegen = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | CharacterStamina") float RegenDelay = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | CharacterStamina") float RegenDelayTimer = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterStamina") bool bIsRegening = false;
	UPROPERTY(EditDefaultsOnly, Category = "Stats|Stamina") float StaminaDrain = 15.f;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterStamina") bool bIsSprinting = false;
	
	//Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Character Stats | CharacterStamina") FOnStaminaChanged OnStaminaChanged; 
	UPROPERTY(BlueprintAssignable) FOnSprintChanged OnSprintChanged;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Mana------------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Mana Functions
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterMana") void OnManaChange(float NewMana);
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterMana") void OnManaRegen(float DeltaTime);
	
	//Mana variables
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterMana") float CurrentMana = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterMana") float MaxMana = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | CharacterMana") float ManaRegen = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | CharacterMana") float ManaDelay = 1.2f;
	UPROPERTY(EditDefaultsOnly, Category = "Character Stats | CharacterMana") float ManaRegenTimer = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Character Stats | CharacterMana") bool bIsManaRegening = false;
	
	//Mana Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Character Stats | CharacterMana") FOnFPChanged OnFPChanged; 
private:
	UFUNCTION() void PlayerLevelUp();
	UFUNCTION() static int64 CalculateXpCostForNextLevel(int32 Level);
};
