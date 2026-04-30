// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBaseStatsComp.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHealthChanged, float,  CurrentHealth, float,  MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, bool, IsDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyHitReact);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEnemyBaseStatsComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBaseStatsComp();
	UFUNCTION() void StartHitStun();
	UFUNCTION() void EndHitStun();
	UFUNCTION() void PlayHitreact();
	UFUNCTION() void PlayDeathAndDestroy();

	//anim montages
	UPROPERTY(EditAnywhere, Category = "Enemy | Stunned") UAnimMontage* hitreactMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")UAnimMontage* DeathMontage;

	//stunned variables
	UPROPERTY(EditAnywhere, Category = "Enemy | Stunned") float HitStunDuration = 0.5f;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy | Stunned")bool bIsStunned = false;

	//Timers
	UPROPERTY() FTimerHandle HitStunTimerHandle;

	//Delegates
	UPROPERTY(BlueprintAssignable, Category = "Combat") FOnEnemyHitReact OnHitReact;

	//destroyables
	UPROPERTY(BlueprintReadWrite, Category = "Combat") AActor* EquippedWeapon;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable) FOnEnemyHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable) FOnEnemyDeath OnDeathChanged;
	UFUNCTION(BlueprintCallable) void EnemyHealthChange(float DamageToTake);
	UFUNCTION(BlueprintCallable) void EnemyDeath();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Death") float XpToGive = 20;
	UPROPERTY(EditAnywhere, Category = "Ai Enemy | Stats") float EnemyCurrentHealth = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Ai Enemy | Stats") float EnemyMaxHealth = 100.0f;
	
private:
	UPROPERTY() bool bIsEnemyDead = false;
};
