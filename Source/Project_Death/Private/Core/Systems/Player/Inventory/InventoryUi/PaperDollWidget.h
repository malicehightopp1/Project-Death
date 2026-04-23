#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaperDollWidget.generated.h"

UCLASS()
class UPaperDollWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Bind these in UMG to your slot widgets in the hierarchy
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Helmet;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Chestplate;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Leggings;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Boots;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Gloves;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_MainHand;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Belt;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_HandAcc;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_HeadAcc;
    UPROPERTY(meta = (BindWidget)) class UEquipmentSlotWidget* Slot_Charm;

    // Stat display widgets — bind in UMG
    UPROPERTY(meta = (BindWidget)) class UTextBlock* Text_TotalArmour;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* Text_AttackSpeed;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* Text_BonusHealth;

    // Called once from your character or HUD to wire everything up
    UFUNCTION(BlueprintCallable, Category = "Paper Doll")
    void InitializePaperDoll(class UEquipmentManager* InEquipmentManager,
                             class UInventoryManager* InInventoryManager);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY() class UEquipmentManager* EquipmentManager;
    UPROPERTY() class UInventoryManager* InventoryManager;

    // Bound to OnEquipmentChanged delegate
    UFUNCTION()
    void OnEquipmentChanged();

    // Refreshes every slot widget and stat readouts
    void RefreshAllSlots();

    // Pushes the right SlotType into each slot widget on init
    void InitializeSlotTypes();
};