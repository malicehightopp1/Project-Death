#include "EquipmentSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Core/Systems/Items/ItemData.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"

void UEquipmentSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Wire the button click to OnSlotClicked
    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UEquipmentSlotWidget::OnSlotClicked);
    }
}

void UEquipmentSlotWidget::RefreshSlot()
{
    if (!EquipmentManager) return;

    bIsOccupied = EquipmentManager->IsSlotOccupied(SlotType);

    if (bIsOccupied)
    {
        // Get the item row name sitting in this slot
        FName RowName = EquipmentManager->GetItemInSlot(SlotType);

        // Pull the icon and name from ItemDataTable via InventoryManager
        if (InventoryManager && InventoryManager->ItemDataTable)
        {
            FItemDataInfo* Data = InventoryManager->ItemDataTable->FindRow<FItemDataInfo>(
                RowName, TEXT(""));

            if (Data)
            {
                ItemIcon = Data->ItemTexture;
                ItemName = FText::FromName(Data->ItemName);

                // Push icon into the UMG Image widget
                if (SlotIcon && ItemIcon)
                {
                    SlotIcon->SetBrushFromTexture(ItemIcon);
                    SlotIcon->SetVisibility(ESlateVisibility::Visible);
                }
            }
        }
    }
    else
    {
        // Slot is empty — clear the icon
        ItemIcon = nullptr;
        ItemName = FText::GetEmpty();

        if (SlotIcon)
        {
            SlotIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UEquipmentSlotWidget::OnSlotClicked()
{
    if (!EquipmentManager) return;

    // Only do something if there is an item here
    if (!bIsOccupied) return;

    // Unequip sends it straight back to inventory
    EquipmentManager->UnequipItem(SlotType);

    // RefreshSlot is NOT called here directly
    // The paper doll widget listens to OnEquipmentChanged and refreshes all slots
}