// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUI.generated.h"


UCLASS()
class UEnemyUI : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override; //treated as begin play starts when added to viewport

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget)) class UProgressBar* EnemyHealthBar;
	UFUNCTION(BlueprintCallable, Category = "Stats | Health") void OnHealthChanged(float NewHealth, float MaxHealth);

	
};
