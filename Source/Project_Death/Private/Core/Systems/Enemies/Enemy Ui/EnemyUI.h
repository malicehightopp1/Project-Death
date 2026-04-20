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
	UFUNCTION(BlueprintCallable, Category = "EnemyUI") void InitwithOwner(AActor* InownerActor);
	UFUNCTION(BlueprintCallable, Category = "Stats | Health") void OnHealthChanged(float NewHealth, float MaxHealth);
	
	UPROPERTY() AActor* OwnerActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget)) class UProgressBar* EnemyHealthBar;

};
