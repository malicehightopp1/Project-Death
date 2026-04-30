// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBaseStatsComp.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStatRow : public FTableRowBase
{
	GENERATED_BODY()

	//base stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float EnemyMaxhealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float EnemyBaseDamage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float EnemyXpToGive = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float HitStunDuration = 0.3;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Delegates ------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHealthChanged, float,  CurrentHealth, float,  MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, bool, IsDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyHitReact);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEnemyBaseStatsComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyBaseStatsComp();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// functions ------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//stun functions
	UFUNCTION() void StartHitStun();
	UFUNCTION() void EndHitStun();
	UFUNCTION() void PlayHitreact();

	//death functions
	UFUNCTION() void PlayDeathAndDestroy();
	UFUNCTION(BlueprintCallable) void EnemyDeath();

	//enemy health 
	UFUNCTION(BlueprintCallable) void EnemyHealthChange(float DamageToTake);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// animations -----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//anim montages
	UPROPERTY(EditAnywhere, Category = "Enemy | Stunned") UAnimMontage* hitreactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")UAnimMontage* DeathMontage;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// enemy Stun -----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//stunned variables
	UPROPERTY(EditAnywhere, Category = "Enemy | Stunned") float HitStunDuration = 0.5f;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy | Stunned")bool bIsStunned = false;

	//Timers
	UPROPERTY() FTimerHandle HitStunTimerHandle;

	//delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Enemy | Delegates") FOnEnemyHitReact OnHitReact;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Player death ---------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//destroyables
	UPROPERTY(BlueprintReadWrite, Category = "Combat") AActor* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) bool bIsEnemyDead = false;

	//delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Enemy | Delegates") FOnEnemyDeath OnDeathChanged;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Enemy Health ---------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//base health stats
	UPROPERTY(EditAnywhere, Category = "Ai Enemy | Stats") float EnemyCurrentHealth = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Ai Enemy | Stats") float EnemyMaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Enemy | Enemy Stats") float EnemyBaseDamage = 10.f;
	
	//health delegate
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Enemy | Delegates") FOnEnemyHealthChanged OnHealthChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// enemy xp to give -----------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//base xp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Death") float XpToGive = 20;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Enemy level ----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Enemy Stats") int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Enemy Stats")UDataTable* EnemyStatTable;

	UFUNCTION(BlueprintCallable, Category = "Enemy | Enemy Stats") void InitStatsFromLevel(int32 Level);

	UFUNCTION(BlueprintPure,  Category = "Enemy | Enemy Stats") static float CalculateEnemyHealth(float Level);
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	//enemy death
};
