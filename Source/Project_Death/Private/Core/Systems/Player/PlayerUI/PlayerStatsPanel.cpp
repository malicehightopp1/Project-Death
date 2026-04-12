// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Systems/Player/PlayerUI/PlayerStatsPanel.h"
#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"	
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"

UPlayerStatsPanel::UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) {}

void UPlayerStatsPanel::NativeConstruct()
{
    Super::NativeConstruct();

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->OnPossessedPawnChanged.AddDynamic(this, &UPlayerStatsPanel::OnPawnChanged);
        if (APawn* Pawn = PC->GetPawn())
        {
            BindToStatsComp(Pawn);
        }
    }
}

void UPlayerStatsPanel::NativeDestruct()
{
    UnbindFromStatsComp();
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->OnPossessedPawnChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnPawnChanged);
    }
    Super::NativeDestruct();
}

void UPlayerStatsPanel::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    UnbindFromStatsComp();
    if (NewPawn) BindToStatsComp(NewPawn);
}

void UPlayerStatsPanel::BindToStatsComp(APawn* Pawn)
{
    StatsComp = Pawn->FindComponentByClass<UCharacterStatsComp>();
    if (!StatsComp) return;

    StatsComp->OnLevelChange.AddDynamic(this, &UPlayerStatsPanel::OnLevelChanged);
    StatsComp->OnXpChanged.AddDynamic(this, &UPlayerStatsPanel::OnXpChanged);
    StatsComp->OnStatPointsChanged.AddDynamic(this, &UPlayerStatsPanel::OnStatPointsChanged);
    StatsComp->OnAttributesChanged.AddDynamic(this, &UPlayerStatsPanel::OnAttributesChanged);

    BuildAttributeRows();

    // Force initial refresh
    OnLevelChanged(StatsComp->CharacterLevel);
    OnXpChanged(StatsComp->CurrentXp, StatsComp->MaxXp);
    OnStatPointsChanged(StatsComp->UnspentStatPoints);
}

void UPlayerStatsPanel::UnbindFromStatsComp()
{
    if (!StatsComp) return;
    StatsComp->OnLevelChange.RemoveDynamic(this, &UPlayerStatsPanel::OnLevelChanged);
    StatsComp->OnXpChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnXpChanged);
    StatsComp->OnStatPointsChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnStatPointsChanged);
    StatsComp->OnAttributesChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnAttributesChanged);
    StatsComp = nullptr;
}

void UPlayerStatsPanel::BuildAttributeRows()
{
        if (!StatRowContainer) return;

    StatRowContainer->ClearChildren();
    SpendButtons.Empty();

    TArray<TPair<FName, FString>> AttributeList =
    {
        { "Vitality",  "Vitality"  },
        { "Endurance", "Endurance" },
        { "Mind",      "Mind"      },
        { "Strength",  "Strength"  },
        { "Dexterity", "Dexterity" },
    };

    for (int32 i = 0; i < AttributeList.Num(); i++)
    {
        UHorizontalBox* Row = NewObject<UHorizontalBox>(this);

        UTextBlock* NameText = NewObject<UTextBlock>(this);
        NameText->SetText(FText::FromString(AttributeList[i].Value));
        UHorizontalBoxSlot* NameSlot = Row->AddChildToHorizontalBox(NameText);
        NameSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

        UTextBlock* LevelText = NewObject<UTextBlock>(this);
        LevelText->SetText(FText::AsNumber(1));
        Row->AddChildToHorizontalBox(LevelText);

        UTextBlock* ValueText = NewObject<UTextBlock>(this);
        ValueText->SetText(FText::FromString(TEXT("—")));
        UHorizontalBoxSlot* ValueSlot = Row->AddChildToHorizontalBox(ValueText);
        ValueSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

        UButton* SpendBtn = NewObject<UButton>(this);
        UTextBlock* BtnLabel = NewObject<UTextBlock>(this);
        BtnLabel->SetText(FText::FromString(TEXT("+")));
        SpendBtn->AddChild(BtnLabel);

        // Bind based on index instead of stored function pointer
        switch (i)
        {
            case 0: SpendBtn->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendVitality);  break;
            case 1: SpendBtn->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendEndurance); break;
            case 2: SpendBtn->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendMind);      break;
            case 3: SpendBtn->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendStrength);  break;
            case 4: SpendBtn->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendDexterity); break;
            default: break;
        }

        Row->AddChildToHorizontalBox(SpendBtn);
        StatRowContainer->AddChild(Row);
        SpendButtons.Add(SpendBtn);
    }

    RefreshAllRows();
}

void UPlayerStatsPanel::RefreshAllRows()
{
    if (!StatsComp || !StatRowContainer) return;

    // Attribute levels in the same order as BuildAttributeRows
    TArray<int32> Levels =
    {
        StatsComp->Attributes.Vitality,
        StatsComp->Attributes.Endurance,
        StatsComp->Attributes.Mind,
        StatsComp->Attributes.Strength,
        StatsComp->Attributes.Dexterity,
    };

    TArray<FName> Names = { "Vitality", "Endurance", "Mind", "Strength", "Dexterity" };

    const bool bCanSpendAny = StatsComp->UnspentStatPoints > 0;
    const TArray<UWidget*>& Rows = StatRowContainer->GetAllChildren();

    for (int32 i = 0; i < Rows.Num(); i++)
    {
        UHorizontalBox* Row = Cast<UHorizontalBox>(Rows[i]);
        if (!Row) continue;

        const TArray<UWidget*>& Cells = Row->GetAllChildren();
        // Cells: [0] NameText, [1] LevelText, [2] ValueText, [3] Button
        if (Cells.Num() < 4) continue;

        const int32 CurrentLevel = Levels[i];

        // Update level number
        if (UTextBlock* LevelText = Cast<UTextBlock>(Cells[1]))
        {
            LevelText->SetText(FText::AsNumber(CurrentLevel));
        }

        // Update derived value preview from data table
        if (UTextBlock* ValueText = Cast<UTextBlock>(Cells[2]))
        {
            FString Preview = TEXT("—");
            if (UDataTable* Table = StatsComp->AttributeStatTable)
            {
                const FAttributeStatRow* Cur  = Table->FindRow<FAttributeStatRow>(FName(*FString::FromInt(CurrentLevel)),     TEXT(""));
                const FAttributeStatRow* Next = Table->FindRow<FAttributeStatRow>(FName(*FString::FromInt(CurrentLevel + 1)), TEXT(""));

                if (Cur)
                {
                    if      (Names[i] == "Vitality")  Preview = FString::Printf(TEXT("HP:  %.0f → %.0f"), Cur->VitalityHP,       Next ? Next->VitalityHP       : Cur->VitalityHP);
                    else if (Names[i] == "Endurance")  Preview = FString::Printf(TEXT("STA: %.0f → %.0f"), Cur->EnduranceStamina, Next ? Next->EnduranceStamina : Cur->EnduranceStamina);
                    else if (Names[i] == "Mind")       Preview = FString::Printf(TEXT("MP:  %.0f → %.0f"), Cur->MindMana,         Next ? Next->MindMana         : Cur->MindMana);
                    else if (Names[i] == "Strength")   Preview = FString::Printf(TEXT("DMG: %.0f → %.0f"), Cur->StrengthDamage,   Next ? Next->StrengthDamage   : Cur->StrengthDamage);
                    else if (Names[i] == "Dexterity")  Preview = FString::Printf(TEXT("SPD: %.2f → %.2f"), Cur->DexteritySpeed,   Next ? Next->DexteritySpeed   : Cur->DexteritySpeed);
                }
            }
            ValueText->SetText(FText::FromString(Preview));
        }

        // Toggle + button
        if (SpendButtons.IsValidIndex(i))
        {
            SpendButtons[i]->SetIsEnabled(bCanSpendAny && CurrentLevel < 99);
        }
    }
}

// --- Delegate callbacks ---

void UPlayerStatsPanel::OnAttributesChanged(FCharacterAttributes NewAttributes)
{
    RefreshAllRows();
}

void UPlayerStatsPanel::OnStatPointsChanged(int32 NewStatPoints)
{
    if (PlayerStatPoints)
    {
        FString Display = NewStatPoints > 0
            ? FString::Printf(TEXT("Points: %d ▲"), NewStatPoints)
            : FString::Printf(TEXT("Points: %d"),   NewStatPoints);
        PlayerStatPoints->SetText(FText::FromString(Display));
    }
    RefreshAllRows(); // re-evaluate button states
}

void UPlayerStatsPanel::OnLevelChanged(int32 NewCharacterLevel)
{
    if (PlayerCurrentLevel)
    {
        PlayerCurrentLevel->SetText(FText::Format(
            NSLOCTEXT("StatsPanel", "Level", "Level: {0}"),
            FText::AsNumber(NewCharacterLevel)));
    }
}

void UPlayerStatsPanel::OnXpChanged(float NewXp, float MaxXp)
{
    if (PlayerXp)
    {
        PlayerXp->SetText(FText::Format(
            NSLOCTEXT("StatsPanel", "XP", "{0} / {1} XP"),
            FText::AsNumber((int32)NewXp), FText::AsNumber((int32)MaxXp)));
    }
    if (PlayerXpBar)
    {
        PlayerXpBar->SetPercent(MaxXp > 0.f ? NewXp / MaxXp : 0.f);
    }
}

// --- Spend handlers ---

void UPlayerStatsPanel::OnSpendVitality()  { if (StatsComp) StatsComp->SpendStatPoint("Vitality");  }
void UPlayerStatsPanel::OnSpendEndurance() { if (StatsComp) StatsComp->SpendStatPoint("Endurance"); }
void UPlayerStatsPanel::OnSpendMind()      { if (StatsComp) StatsComp->SpendStatPoint("Mind");      }
void UPlayerStatsPanel::OnSpendStrength()  { if (StatsComp) StatsComp->SpendStatPoint("Strength");  }
void UPlayerStatsPanel::OnSpendDexterity() { if (StatsComp) StatsComp->SpendStatPoint("Dexterity"); }