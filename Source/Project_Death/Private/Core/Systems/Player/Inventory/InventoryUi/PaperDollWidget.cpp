#include "PaperDollWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"

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

    EquipmentManager->OnEquipmentChanged.AddDynamic(
        this, &UPaperDollWidget::OnEquipmentChanged);

    RefreshAllSlots();
}

void UPaperDollWidget::OnEquipmentChanged()
{
    RefreshAllSlots();
}

void UPaperDollWidget::RefreshAllSlots()
{

    if (!WidgetTree) return;

    WidgetTree->ForEachWidget([&](UWidget* Widget)
    {
        if (!Widget) return;
        
        UFunction* PopulateFunc = Widget->FindFunction(TEXT("PopulateSpot"));
        if (!PopulateFunc) return;

        // Only call on slots marked as equipment slots
        FBoolProperty* IsEquipProp = FindFProperty<FBoolProperty>(
            Widget->GetClass(), TEXT("bIsEquipmentSlot"));
        if (!IsEquipProp) return;

        bool bIsEquip = IsEquipProp->GetPropertyValue_InContainer(Widget);
        if (!bIsEquip) return;
        
        struct FPopulateParams
        {
            FName  ItemRowName = NAME_None;
            int32  Quantity    = 0;
            bool   bIsEmpty    = true;
            int32  SlotIndex   = 0;
            UInventoryManager* InvManager = nullptr;
        };

        FPopulateParams Params;
        Params.InvManager = InventoryManager;

        Widget->ProcessEvent(PopulateFunc, &Params);
    });
}