#include "EquipmentSlotWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Core/Systems/Items/ItemData.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"

void UEquipmentSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

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
        FName RowName = EquipmentManager->GetItemInSlot(SlotType);

        if (InventoryManager && InventoryManager->ItemDataTable)
        {
            FItemDataInfo* Data = InventoryManager->ItemDataTable->FindRow<FItemDataInfo>(
                RowName, TEXT(""));

            if (Data)
            {
                ItemIcon = Data->ItemTexture;
                ItemName = FText::FromName(Data->ItemName);

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

    if (!bIsOccupied) return;

    EquipmentManager->UnequipItem(SlotType);
}