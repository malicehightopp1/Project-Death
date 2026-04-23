#include "PaperDollWidget.h"
#include "Components/TextBlock.h"
#include "EquipmentSlotWidget.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"

void UPaperDollWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPaperDollWidget::InitializePaperDoll(UEquipmentManager* InEquipmentManager,
                                            UInventoryManager* InInventoryManager)
{
    EquipmentManager = InEquipmentManager;
    InventoryManager = InInventoryManager;

    if (!EquipmentManager) return;

    // Tell each slot widget which slot it represents and give it manager refs
    InitializeSlotTypes();

    // Bind to the equipment changed delegate — same pattern as your inventory UI
    EquipmentManager->OnEquipmentChanged.AddDynamic(this, &UPaperDollWidget::OnEquipmentChanged);

    // Do a first pass refresh so slots show correctly on open
    RefreshAllSlots();
}

void UPaperDollWidget::InitializeSlotTypes()
{
    // Each slot widget needs to know what slot it represents
    // and have references to the managers
    auto Setup = [&](UEquipmentSlotWidget* SlotWidget, EEquipmentSlot SlotType)
    {
        if (!SlotWidget) return;
        SlotWidget->SlotType        = SlotType;
        SlotWidget->EquipmentManager = EquipmentManager;
        SlotWidget->InventoryManager = InventoryManager;
    };

    Setup(Slot_Helmet,     EEquipmentSlot::Helmet);
    Setup(Slot_Chestplate, EEquipmentSlot::Chestplate);
    Setup(Slot_Leggings,   EEquipmentSlot::Leggings);
    Setup(Slot_Boots,      EEquipmentSlot::Boots);
    Setup(Slot_Gloves,     EEquipmentSlot::Gloves);
    Setup(Slot_MainHand,   EEquipmentSlot::MainHand);
    Setup(Slot_Belt,       EEquipmentSlot::Belt);
    Setup(Slot_HandAcc,    EEquipmentSlot::HandAcc);
    Setup(Slot_HeadAcc,    EEquipmentSlot::HeadAcc);
    Setup(Slot_Charm,      EEquipmentSlot::Charm);
}

void UPaperDollWidget::OnEquipmentChanged()
{
    // Fires every time EquipmentManager broadcasts
    RefreshAllSlots();
}

void UPaperDollWidget::RefreshAllSlots()
{
    // Refresh every slot widget
    if (Slot_Helmet)     Slot_Helmet->RefreshSlot();
    if (Slot_Chestplate) Slot_Chestplate->RefreshSlot();
    if (Slot_Leggings)   Slot_Leggings->RefreshSlot();
    if (Slot_Boots)      Slot_Boots->RefreshSlot();
    if (Slot_Gloves)     Slot_Gloves->RefreshSlot();
    if (Slot_MainHand)   Slot_MainHand->RefreshSlot();
    if (Slot_Belt)       Slot_Belt->RefreshSlot();
    if (Slot_HandAcc)    Slot_HandAcc->RefreshSlot();
    if (Slot_HeadAcc)    Slot_HeadAcc->RefreshSlot();
    if (Slot_Charm)      Slot_Charm->RefreshSlot();

    // Refresh stat readouts
    if (!EquipmentManager) return;

    if (Text_TotalArmour)
    {
        int32 Armour = EquipmentManager->GetTotalArmourDefense();
        Text_TotalArmour->SetText(FText::FromString(
            FString::Printf(TEXT("Armour: %d"), Armour)));
    }
    if (Text_AttackSpeed)
    {
        float Speed = EquipmentManager->GetEquippedWeaponAttackSpeed();
        Text_AttackSpeed->SetText(FText::FromString(FString::Printf(TEXT("Speed: %.2f"), Speed)));
    }
    if (Text_BonusHealth)
    {
        
    }
}