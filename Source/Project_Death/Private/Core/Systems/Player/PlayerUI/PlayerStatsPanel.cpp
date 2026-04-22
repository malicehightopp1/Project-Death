#include "Core/Systems/Player/PlayerUI/PlayerStatsPanel.h"

UPlayerStatsPanel::UPlayerStatsPanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UPlayerStatsPanel::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind spend buttons
    if (SpendBtn_Vitality) SpendBtn_Vitality->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendVitality);
    if (SpendBtn_Endurance) SpendBtn_Endurance->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendEndurance);
    if (SpendBtn_Mind) SpendBtn_Mind->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendMind);
    if (SpendBtn_Strength)  SpendBtn_Strength->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendStrength);
    if (SpendBtn_Dexterity) SpendBtn_Dexterity->OnClicked.AddDynamic(this, &UPlayerStatsPanel::OnSpendDexterity);

    if (APlayerController* PC = GetOwningPlayer()) //get the player that owns this widget 
    {
        PC->OnPossessedPawnChanged.AddDynamic(this, &UPlayerStatsPanel::OnPawnChanged); //This delegate fires whenever a new pawn is detected
        if (APawn* Pawn = PC->GetPawn()) //returns the pawn the controller is currently possessing
        {
            BindToStatsComp(Pawn); //binds the pawn to my stats component
        }
    }
}

void UPlayerStatsPanel::NativeDestruct()
{
    UnbindFromStatsComp();
    if (APlayerController* PC = GetOwningPlayer()) //same thing as above but removing it instead 
    {
        PC->OnPossessedPawnChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnPawnChanged);
    }
    Super::NativeDestruct();
}

//if new pawn detected unbind and rebind to the new pawn
void UPlayerStatsPanel::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
    UnbindFromStatsComp();
    if (NewPawn) BindToStatsComp(NewPawn);
}

//binding all the delegates to the xp and attributes
void UPlayerStatsPanel::BindToStatsComp(APawn* Pawn)
{
    StatsComp = Pawn->FindComponentByClass<UCharacterStatsComp>(); //grabbing the player stats component
    if (!StatsComp) return;

    StatsComp->OnLevelChange.AddDynamic(this, &UPlayerStatsPanel::OnLevelChanged);
    StatsComp->OnXpChanged.AddDynamic(this, &UPlayerStatsPanel::OnXpChanged);
    StatsComp->OnStatPointsChanged.AddDynamic(this, &UPlayerStatsPanel::OnStatPointsChanged);
    StatsComp->OnAttributesChanged.AddDynamic(this, &UPlayerStatsPanel::OnAttributesChanged);

    // Force initial refresh
    OnLevelChanged(StatsComp->CharacterLevel);
    OnXpChanged(StatsComp->CurrentXp, StatsComp->MaxXp);
    OnStatPointsChanged(StatsComp->UnspentStatPoints);

    RefreshAllRows(); //called immediatly to update
}

//Unbinding all the stats delegates
void UPlayerStatsPanel::UnbindFromStatsComp()
{
    if (!StatsComp) return;
    StatsComp->OnLevelChange.RemoveDynamic(this, &UPlayerStatsPanel::OnLevelChanged);
    StatsComp->OnXpChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnXpChanged);
    StatsComp->OnStatPointsChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnStatPointsChanged);
    StatsComp->OnAttributesChanged.RemoveDynamic(this, &UPlayerStatsPanel::OnAttributesChanged);
    StatsComp = nullptr;
}

//Refreshing the ui when changes happen *SEE IT AS A GLORIFIED DELEGATE CALLED WHENEVER UI NEEDS TO BE UPDATED*
void UPlayerStatsPanel::RefreshRow(UTextBlock* LevelText, UTextBlock* ValueText,UButton* SpendBtn, FName AttributeName, int32 CurrentLevel, bool bCanSpend)
{
    if (LevelText) //setting the players level
    {
        FString Display = FString::Printf(TEXT("%s: %d "), *AttributeName.ToString(),  CurrentLevel); //needs to be pointer to attribute name because FName cannot be directly converted
        LevelText->SetText(FText::FromString(Display));
    }

    if (ValueText)
    {
        FString Preview = TEXT("—");
        if (UDataTable* Table = StatsComp ? StatsComp->AttributeStatTable : nullptr)
        {
            //the date of current level value and what the next levels value is
            const FAttributeStatRow* Cur  = Table->FindRow<FAttributeStatRow>(FName(*FString::FromInt(CurrentLevel)), TEXT(""));
            const FAttributeStatRow* Next = Table->FindRow<FAttributeStatRow>(FName(*FString::FromInt(CurrentLevel + 1)), TEXT(""));

            //Setting the current and next level of stats to the ui
            if (Cur)
            {
                if (AttributeName == "Vitality")  Preview = FString::Printf(TEXT("HP : %.0f -> %.0f"), Cur->VitalityHP, Next ? Next->VitalityHP : Cur->VitalityHP);
                else if (AttributeName == "Endurance") Preview = FString::Printf(TEXT("STA : %.0f -> %.0f"), Cur->EnduranceStamina, Next ? Next->EnduranceStamina : Cur->EnduranceStamina);
                else if (AttributeName == "Mind") Preview = FString::Printf(TEXT("MP : %.0f -> %.0f"), Cur->MindMana, Next ? Next->MindMana : Cur->MindMana);
                else if (AttributeName == "Strength")  Preview = FString::Printf(TEXT("DMG : %.0f -> %.0f"), Cur->StrengthDamage, Next ? Next->StrengthDamage : Cur->StrengthDamage);
                else if (AttributeName == "Dexterity") Preview = FString::Printf(TEXT("SPD : %.2f -> %.2f"), Cur->DexteritySpeed, Next ? Next->DexteritySpeed : Cur->DexteritySpeed);
            }
        }
        ValueText->SetText(FText::FromString(Preview)); //actually setting the preview value
    }

    //if level is below 99 then have the button enabled stopping from going over 99
    if (SpendBtn)
    {
        SpendBtn->SetIsEnabled(bCanSpend && CurrentLevel < 99);
    }
}

//The one aboce is the defintion of refreshing rows below is the call to actually refresh them for all stats
void UPlayerStatsPanel::RefreshAllRows()
{
    if (!StatsComp) return;

    const bool bCanSpend = StatsComp->UnspentStatPoints > 0;

    RefreshRow(LevelText_Vitality, ValueText_Vitality,SpendBtn_Vitality, "Vitality", StatsComp->Attributes.Vitality, bCanSpend);
    RefreshRow(LevelText_Endurance, ValueText_Endurance,SpendBtn_Endurance, "Endurance", StatsComp->Attributes.Endurance, bCanSpend);
    RefreshRow(LevelText_Mind, ValueText_Mind, SpendBtn_Mind,"Mind", StatsComp->Attributes.Mind, bCanSpend);
    RefreshRow(LevelText_Strength,  ValueText_Strength,  SpendBtn_Strength,  "Strength",  StatsComp->Attributes.Strength, bCanSpend);
    RefreshRow(LevelText_Dexterity, ValueText_Dexterity, SpendBtn_Dexterity, "Dexterity", StatsComp->Attributes.Dexterity, bCanSpend);
}

void UPlayerStatsPanel::OnAttributesChanged(FCharacterAttributes NewAttributes) { RefreshAllRows(); }

//Changing the stat points of the player
void UPlayerStatsPanel::OnStatPointsChanged(int32 NewStatPoints)
{
    if (PlayerStatPoints) 
    {
        //U+25B2 for getting the triangle im using for stat poitns the unicode is built in just need the code most the time
        FString Display = NewStatPoints > 0 ? FString::Printf(TEXT("Points: %d "), NewStatPoints) : FString::Printf(TEXT("Points: %d"), NewStatPoints);
        PlayerStatPoints->SetText(FText::FromString(Display));
    }
    RefreshAllRows();
}

//call for level change 
void UPlayerStatsPanel::OnLevelChanged(int32 NewCharacterLevel)
{
    if (PlayerCurrentLevel)
    {
        PlayerCurrentLevel->SetText(FText::Format(NSLOCTEXT("StatsPanel", "Level", "Level: {0}"), FText::AsNumber(NewCharacterLevel)));
    }
}

//call for player xp change
void UPlayerStatsPanel::OnXpChanged(float NewXp, float MaxXp)
{
    if (PlayerXp)
    {
        PlayerXp->SetText(FText::Format(NSLOCTEXT("StatsPanel", "XP", "{0} / {1} XP"), FText::AsNumber((int32)NewXp), FText::AsNumber((int32)MaxXp)));
    }
    if (PlayerXpBar)
    {
        PlayerXpBar->SetPercent(MaxXp > 0.f ? NewXp / MaxXp : 0.f);
    }
}

//all calls for the stats going to the stat component and using the datta and function to spend the point
void UPlayerStatsPanel::OnSpendVitality() { if (StatsComp) StatsComp->SpendStatPoint("Vitality"); }
void UPlayerStatsPanel::OnSpendEndurance() { if (StatsComp) StatsComp->SpendStatPoint("Endurance"); }
void UPlayerStatsPanel::OnSpendMind() { if (StatsComp) StatsComp->SpendStatPoint("Mind"); }
void UPlayerStatsPanel::OnSpendStrength() { if (StatsComp) StatsComp->SpendStatPoint("Strength"); }
void UPlayerStatsPanel::OnSpendDexterity() { if (StatsComp) StatsComp->SpendStatPoint("Dexterity"); }