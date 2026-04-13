// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "PlayerStatsPanel.generated.h"

class UCharacterStatsComp;
class UProgressBar;
class UTextBlock;
class UVerticalBox;
class UButton;

UCLASS()
class PROJECT_DEATH_API UPlayerStatsPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // -------------------------------------------------------------------------
    // Bound Widget References
    // -------------------------------------------------------------------------

    UPROPERTY(meta = (BindWidget))TObjectPtr<UTextBlock> PlayerCurrentLevel;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> PlayerXp;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UProgressBar> PlayerXpBar;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> PlayerStatPoints;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UVerticalBox> StatRowContainer;

    // -------------------------------------------------------------------------
    // Internal State
    // -------------------------------------------------------------------------

    UPROPERTY() TObjectPtr<UCharacterStatsComp> StatsComp;

    UPROPERTY() TArray<TObjectPtr<UButton>> SpendButtons; //used for creating the buttons that use the statcomp to upgrade the attrubutes

    // -------------------------------------------------------------------------
    // Pawn / Component Binding
    // -------------------------------------------------------------------------

    void BindToStatsComp(APawn* Pawn);
    void UnbindFromStatsComp();

    UFUNCTION()
    void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);

    // -------------------------------------------------------------------------
    // UI Building
    // -------------------------------------------------------------------------

    void BuildAttributeRows();
    void RefreshAllRows();

    // -------------------------------------------------------------------------
    // Delegate Callbacks
    // -------------------------------------------------------------------------

    UFUNCTION() void OnLevelChanged(int32 NewCharacterLevel);

    UFUNCTION() void OnXpChanged(float NewXp, float MaxXp);

    UFUNCTION() void OnStatPointsChanged(int32 NewStatPoints);

    UFUNCTION() void OnAttributesChanged(FCharacterAttributes NewAttributes);

    // -------------------------------------------------------------------------
    // Stat-Spend Button Handlers
    // -------------------------------------------------------------------------

    UFUNCTION() void OnSpendVitality();

    UFUNCTION() void OnSpendEndurance();

    UFUNCTION() void OnSpendMind();

    UFUNCTION() void OnSpendStrength();

    UFUNCTION() void OnSpendDexterity();
};