// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatsPanel.generated.h"

struct FCharacterAttributes;
class UButton;
class UCharacterStatsComp;
class UVerticalBox;

UCLASS()
class UPlayerStatsPanel : public UUserWidget
{
	GENERATED_BODY()
	UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer); //This is what a userwidget constructor looks like
	virtual void NativeConstruct() override; //treated as begin play starts when added to viewport
	virtual void NativeDestruct() override;
protected:

	// --- New: container where rows are spawned ---
	UPROPERTY(meta=(BindWidget)) UVerticalBox* StatRowContainer;

	// --- New: set this in BP defaults to your WBP_StatRow ---
	UPROPERTY(EditDefaultsOnly, Category="UI") TSubclassOf<UUserWidget> StatRowWidgetClass;
	//PlayerLevel 
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerCurrentLevel;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerXp;
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional)) class UProgressBar* PlayerXpBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UTextBlock* PlayerStatPoints;
	
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnLevelChanged(int32 NewCharacterLevel);
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnXpChanged(float NewXp, float MaxXp);
	UFUNCTION(BlueprintCallable, Category = "Stats | Level") void OnStatPointsChanged(int32 NewStatPoints);
	
private:
	UPROPERTY() UCharacterStatsComp* StatsComp = nullptr;

	// Stores the + buttons so we can toggle them on/off
	UPROPERTY() TArray<UButton*> SpendButtons;

	void BindToStatsComp(APawn* Pawn);
	void UnbindFromStatsComp();
	void BuildAttributeRows();
	void RefreshAllRows();

	UFUNCTION() void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);
	UFUNCTION() void OnAttributesChanged(FCharacterAttributes NewAttributes);

	// One click handler per attribute — bound dynamically
	UFUNCTION() void OnSpendVitality();
	UFUNCTION() void OnSpendEndurance();
	UFUNCTION() void OnSpendMind();
	UFUNCTION() void OnSpendStrength();
	UFUNCTION() void OnSpendDexterity();
};
