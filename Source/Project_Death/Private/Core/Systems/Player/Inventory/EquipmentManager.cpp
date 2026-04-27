#include "EquipmentManager.h"

#include "InventoryManager.h"
#include "Core/Systems/Player/Base/PlayerStats/CharacterStatsComp.h"
#include "Core/Systems/Items/ItemData.h"

UEquipmentManager::UEquipmentManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentManager::BeginPlay()
{
    Super::BeginPlay();

    // Find sibling components on the same actor
    InventoryManager = GetOwner()->FindComponentByClass<UInventoryManager>();
    StatsComp = GetOwner()->FindComponentByClass<UCharacterStatsComp>();
}

// ========================================================================================================
// ------ Core --------------------------------------------------------------------------------------------
// ========================================================================================================

bool UEquipmentManager::EquipItem(FName ItemRowName)
{
    if (!InventoryManager || !ItemDataTable) return false;

    EEquipmentSlot TargetSlot = GetSlotForItem(ItemRowName);
    if (TargetSlot == EEquipmentSlot::None) return false;

    // If slot occupied, send old item back to inventory first
    if (IsSlotOccupied(TargetSlot))
    {
        FName OldItem = EquippedItems[TargetSlot];
        InventoryManager->AddItemToInventory(OldItem, 1);
    }

    InventoryManager->RemoveItemFromInventory(ItemRowName, 1);
    EquippedItems.Add(TargetSlot, ItemRowName);

    PushAccessoryBonusesToStats(); // Recalculate stats after any change
    OnEquipmentChanged.Broadcast();
    return true;
}

bool UEquipmentManager::UnequipItem(EEquipmentSlot Slot)
{
    if (!IsSlotOccupied(Slot) || !InventoryManager) return false;
    if (InventoryManager->IsInventoryFull()) return false;

    FName ItemToReturn = EquippedItems[Slot];
    InventoryManager->AddItemToInventory(ItemToReturn, 1);
    EquippedItems.Remove(Slot);

    PushAccessoryBonusesToStats(); // Recalculate stats after any change
    OnEquipmentChanged.Broadcast();
    return true;
}

bool UEquipmentManager::IsSlotOccupied(EEquipmentSlot Slot)
{
    return EquippedItems.Contains(Slot);
}

FName UEquipmentManager::GetItemInSlot(EEquipmentSlot Slot)
{
    if (!IsSlotOccupied(Slot)) return NAME_None;
    return EquippedItems[Slot];
}

// ========================================================================================================
// ------ Stat Queries ------------------------------------------------------------------------------------
// ========================================================================================================

int32 UEquipmentManager::GetTotalArmourDefense()
{
    if (!ArmourDataTable) return 0;
    
    int32 Total = 0;
    TArray<EEquipmentSlot> ArmourSlots = {
        EEquipmentSlot::Helmet, EEquipmentSlot::Chestplate,
        EEquipmentSlot::Leggings, EEquipmentSlot::Boots, EEquipmentSlot::Gloves
    };

    for (EEquipmentSlot Slot : ArmourSlots)
    {
        if (!IsSlotOccupied(Slot)) continue;
        FArmourDataInfo* Data = ArmourDataTable->FindRow<FArmourDataInfo>(EquippedItems[Slot], TEXT(""));
        if (Data) Total += Data->ArmourDefense;
    }
    return Total;
}

float UEquipmentManager::GetEquippedWeaponAttackSpeed()
{
    if (!WeaponDataTable || !IsSlotOccupied(EEquipmentSlot::MainHand)) return 1.0f;

    FWeaponDataInfo* Data = WeaponDataTable->FindRow<FWeaponDataInfo>(
        EquippedItems[EEquipmentSlot::MainHand], TEXT(""));

    return Data ? Data->AttackSpeed : 1.0f;
}

EWeaponDataType UEquipmentManager::GetEquippedWeaponType()
{
    if (!WeaponDataTable || !IsSlotOccupied(EEquipmentSlot::MainHand))
        return EWeaponDataType::MiscWeapon;

    FWeaponDataInfo* Data = WeaponDataTable->FindRow<FWeaponDataInfo>(
        EquippedItems[EEquipmentSlot::MainHand], TEXT(""));

    return Data ? Data->WeaponType : EWeaponDataType::MiscWeapon;
}

// ========================================================================================================
// ------ Private -----------------------------------------------------------------------------------------
// ========================================================================================================

EEquipmentSlot UEquipmentManager::GetSlotForItem(FName ItemRowName)
{
    if (!ItemDataTable) return EEquipmentSlot::None;
    FItemDataInfo* Data = ItemDataTable->FindRow<FItemDataInfo>(ItemRowName, TEXT(""));
    if (!Data) return EEquipmentSlot::None;
    return Data->SlotType;
}

void UEquipmentManager::PushAccessoryBonusesToStats()
{
    if (!StatsComp || !AccessoryDataTable || !ArmourDataTable) return;

    float TotalHP   = 0.f;
    float TotalStam = 0.f;
    float TotalMana = 0.f;

    // Accessory slots
    TArray<EEquipmentSlot> AccSlots = {
        EEquipmentSlot::Belt, EEquipmentSlot::HandAcc,
        EEquipmentSlot::HeadAcc, EEquipmentSlot::Charm
    };

    for (EEquipmentSlot Slot : AccSlots)
    {
        if (!IsSlotOccupied(Slot)) continue;
        FAccessoryItemData* Data = AccessoryDataTable->FindRow<FAccessoryItemData>(
            EquippedItems[Slot], TEXT(""));
        if (!Data) continue;

        TotalHP   += Data->BonusHealth;
        TotalStam += Data->BonusStamina;
        TotalMana += Data->BonusMana;
    }

    // Armour slots — these also carry bonus stat fields
    TArray<EEquipmentSlot> ArmourSlots = {
        EEquipmentSlot::Helmet, EEquipmentSlot::Chestplate,
        EEquipmentSlot::Leggings, EEquipmentSlot::Boots,
        EEquipmentSlot::Gloves
    };

    for (EEquipmentSlot Slot : ArmourSlots)
    {
        if (!IsSlotOccupied(Slot)) continue;
        FArmourDataInfo* Data = ArmourDataTable->FindRow<FArmourDataInfo>(
            EquippedItems[Slot], TEXT(""));
        if (!Data) continue;

        TotalHP   += Data->BonusHealth;
        TotalStam += Data->BonusStamina;
        TotalMana += Data->BonusMana;
    }

    StatsComp->ApplyEquipmentBonuses(TotalHP, TotalStam, TotalMana);
}