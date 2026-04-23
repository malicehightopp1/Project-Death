#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "PlayerStatsPanel.generated.h"

UCLASS()
class PROJECT_DEATH_API UPlayerStatsPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    /* theres a better way to do this i could do it with one widget each and in a forloop create a widget for each stat *KEEP THIS IN MIND FOR REVISION* */
    // Header info
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerCurrentLevel;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerXp;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* PlayerXpBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PlayerStatPoints;

    // Vitality row
    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText_Vitality;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ValueText_Vitality;

    UPROPERTY(meta = (BindWidget))
    UButton* SpendBtn_Vitality;

    // Endurance row
    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText_Endurance;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ValueText_Endurance;

    UPROPERTY(meta = (BindWidget))
    UButton* SpendBtn_Endurance;

    // Mind row
    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText_Mind;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ValueText_Mind;

    UPROPERTY(meta = (BindWidget))
    UButton* SpendBtn_Mind;

    // Strength row
    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText_Strength;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ValueText_Strength;

    UPROPERTY(meta = (BindWidget))
    UButton* SpendBtn_Strength;

    // Dexterity row
    UPROPERTY(meta = (BindWidget))
    UTextBlock* LevelText_Dexterity;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ValueText_Dexterity;

    UPROPERTY(meta = (BindWidget))
    UButton* SpendBtn_Dexterity;

private:
    UPROPERTY()
    UCharacterStatsComp* StatsComp = nullptr;

    void BindToStatsComp(APawn* Pawn);
    void UnbindFromStatsComp();
    void RefreshAllRows();
    void RefreshRow(UTextBlock* LevelText, UTextBlock* ValueText, UButton* SpendBtn, FName AttributeName, int32 CurrentLevel, bool bCanSpend);

    UFUNCTION() void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);
    UFUNCTION() void OnLevelChanged(int32 NewCharacterLevel);
    UFUNCTION() void OnXpChanged(float NewXp, float MaxXp);
    UFUNCTION() void OnStatPointsChanged(int32 NewStatPoints);
    UFUNCTION() void OnAttributesChanged(FCharacterAttributes NewAttributes);

    UFUNCTION() void OnSpendVitality();
    UFUNCTION() void OnSpendEndurance();
    UFUNCTION() void OnSpendMind();
    UFUNCTION() void OnSpendStrength();
    UFUNCTION() void OnSpendDexterity();
};