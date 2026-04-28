// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBaseStatsComp.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHealthChanged, float,  CurrentHealth, float,  MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, bool, IsDead);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEnemyBaseStatsComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyBaseStatsComp();

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
