// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CurrencyManager.generated.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// delegates-------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrencyChange , int32, NewCurrency);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrencyPickup, int32, Amount, FText, Label);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCurrencyManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCurrencyManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Currency--------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Currency functions
	UFUNCTION(BlueprintCallable, Category = "PlayerCurrency") void OnCurrencyChange(float NewCurrency);

	UFUNCTION(BlueprintCallable, Category = "PlayerStats") float GetPlayerCurrentCurrency(){return CurrentCurrency;}
	//Currency Variables
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats") float CurrentCurrency = 0;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStats") float MaxCurrency = 999999;
	
	//Currency Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Character Stats | Character") FOnCurrencyChange OnCurrencyChanged;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Character Stats | Character") FOnCurrencyPickup OnCurrencyPickup;

};
