// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUI.generated.h"


class UTextBlock;

UCLASS()
class UEnemyUI : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override; //treated as begin play starts when added to viewport

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Health bar visability ------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//visability functions
	UFUNCTION() void ShowHealthBar();
	UFUNCTION() void HideHealthBar();

	//visability variables
	UPROPERTY(EditAnywhere, Category = "UI") float HideDelay = 10.0f;
	UPROPERTY() FTimerHandle HideTimerHandle;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// start up function ----------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//init function
	UFUNCTION(BlueprintCallable, Category = "EnemyUI") void InitwithOwner(AActor* InownerActor);

	//init variable
	UPROPERTY() AActor* OwnerActor = nullptr;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// health UI ------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Health function
	UFUNCTION(BlueprintCallable, Category = "Stats | Health") void OnHealthChanged(float NewHealth, float MaxHealth);

	//health components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget)) class UProgressBar* EnemyHealthBar;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Enemy Level ----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//EnemyLevel function
	UFUNCTION() void UpdateLevelDisplay(int32 Level);
	UPROPERTY(meta = (BindWidget)) UTextBlock* EnemyLevelText;
	

};
