// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UOverlay;
class UCanvasPanel;

UCLASS()
class UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	UPlayerWidget(const FObjectInitializer& ObjectInitializer); //This is what a userwidget constructor looks like
	virtual void NativeConstruct() override; //treated as begin play starts when added to viewport
protected:
	//Base for UI *HOLDERS*
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UCanvasPanel* CanvasPanel;
	
	//Player cursor
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* PlayerCursorImage;
	
	//PlayerLevel 
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerCurrentLevel;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerXp;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional)) class UProgressBar* PlayerXpBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerStatPoints;
	
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnLevelChanged(int32 NewCharacterLevel);
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnXpChanged(float NewXp, float MaxXp);
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnStatPointsChanged(int32 NewStatPoints);
	
	//PlayerHealth
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* HealthText;
	UFUNCTION(BlueprintCallable, Category = "Stats | Health") void OnHealthChanged(float NewHealth, float MaxHealth);
	
	//PlayerStamina 
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UProgressBar* StaminaBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) UTextBlock* StaminaText;
	UFUNCTION(BlueprintCallable, Category = "Stats | Health") void OnStaminaChanged(float NewStamina, float MaxStamina);
	
	//Player Mana 
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UProgressBar* ManaBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) UTextBlock* ManaText;
	UFUNCTION(BlueprintCallable, Category = "Stats | Health") void OnManaChange(float NewMana, float MaxMana);
	
	//Currency 
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* CurrencyText;
	UFUNCTION(BlueprintCallable, Category = "Stats | Currency") void OnCurrencyChange(int32 NewCurrency);


};
