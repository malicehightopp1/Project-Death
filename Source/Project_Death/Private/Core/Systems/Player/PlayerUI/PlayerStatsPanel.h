// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsPanel.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerStatsPanel : public UUserWidget
{
	GENERATED_BODY()
	UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer); //This is what a userwidget constructor looks like
	virtual void NativeConstruct() override; //treated as begin play starts when added to viewport
protected:
	//PlayerLevel 
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerCurrentLevel;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerXp;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional)) class UProgressBar* PlayerXpBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerStatPoints;
	
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnLevelChanged(int32 NewCharacterLevel);
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnXpChanged(float NewXp, float MaxXp);
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnStatPointsChanged(int32 NewStatPoints);
};
