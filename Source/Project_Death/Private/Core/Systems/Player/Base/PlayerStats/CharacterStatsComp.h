// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComp.generated.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Enums-----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class EPlayerAttributes : uint8
{
	Vigor        UMETA(DisplayName = "Vigor"),
	Endurance    UMETA(DisplayName = "Endurance"),
	Strength     UMETA(DisplayName = "Strength"),
	Dexterity    UMETA(DisplayName = "Dexterity"),
	MAX          UMETA(Hidden)
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Structs---------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute",
		meta = (ClampMin = 1, ClampMax = 99))
	int32 Level = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	int32 HardCap = 99;
};

USTRUCT(BlueprintType)
struct FResourcePool
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Resource") float Current = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = "Resource") float Maximum = 100.f;
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = "Resource") float RegenPerSecond  = 0.f;
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = "Resource") float RegenDelay      = 0.f;

	float GetPercent() const { return (Maximum > 0.f) ? (Current / Maximum) : 0.f; }
	bool  IsEmpty()    const { return Current <= 0.f; }
	bool  IsFull()     const { return Current >= Maximum; }
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EPlayerAttributes, Attr, int32, NewLevel);

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
	/// XP -------------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Function
	UFUNCTION(BlueprintCallable, Category = "Character Stats | CharacterXp") void OnXpChange(float mXpAddAmount);
	
	//Variables
	UPROPERTY(BlueprintReadOnly, Category = "Characeter Stats | Level") int32 CharacterLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Characeter Stats | Level") int32 UnspentStatPoints = 0; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats|Level",meta = (ClampMin = 1, ClampMax = 10)) int32 StatPointsPerLevel = 1; //stat points the player gets per level up

	UPROPERTY(BlueprintReadOnly, Category = "Stats|Level") int64 CurrentXp= 0;
	UPROPERTY(BlueprintReadOnly, Category = "Stats|Level") int64 MaxXp = 0;
	
	//delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stats | Level") FOnCharacterLevelUp OnLevelChange; 
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stats | Level") FOnStatPointsChanged OnStatPointsChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Stats | Level") FOnXpChanged OnXpChanged;
private:
	UFUNCTION() void PlayerLevelUp();
	UFUNCTION()
	static int64 CalculateXpCostForNextLevel(int32 Level);
};
