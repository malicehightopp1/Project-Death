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
    InventoryManager = GetOwner()->FindComponentByClass<UInventoryManager>();
    StatsComp = GetOwner()->FindComponentByClass<UCharacterStatsComp>();
}

FEquipmentSlotData* UEquipmentManager::GetSlotByEnum(EEquipmentSlot Slot)
{
    switch (Slot)
    {
        case EEquipmentSlot::Helmet:      return &HeadSlot;
        case EEquipmentSlot::Chestplate:  return &ChestSlot;
        case EEquipmentSlot::Leggings:    return &LegsSlot;
        case EEquipmentSlot::Boots:       return &BootsSlot;
        case EEquipmentSlot::Gloves:      return &GlovesSlot;
        case EEquipmentSlot::MainHand:    return &MainHandSlot;
        case EEquipmentSlot::Belt:        return &BeltSlot;
        case EEquipmentSlot::HandAcc:     return &HandAccSlot;
        case EEquipmentSlot::HeadAcc:     return &HeadAccSlot;
        case EEquipmentSlot::Charm:       return &CharmSlot;
        default:                          return nullptr;
    }
}

const FEquipmentSlotData* UEquipmentManager::GetSlotByEnum(EEquipmentSlot Slot) const
{
    return const_cast<UEquipmentManager*>(this)->GetSlotByEnum(Slot);
}

EEquipmentSlot UEquipmentManager::GetSlotTypeForItem(FName ItemRowName) const
{
    if (!ItemDataTable) return EEquipmentSlot::None;
    const FItemDataInfo* Data = ItemDataTable->FindRow<FItemDataInfo>(ItemRowName, TEXT(""));
    return Data ? Data->SlotType : EEquipmentSlot::None;
}
bool UEquipmentManager::EquipItem(FName ItemRowName)
{
    if (!InventoryManager || !ItemDataTable) return false;

    // Look up which slot this item belongs in
    const FItemDataInfo* ItemData = ItemDataTable->FindRow<FItemDataInfo>(ItemRowName, TEXT(""));
    if (!ItemData) return false;

    EEquipmentSlot TargetSlot = ItemData->SlotType;
    if (TargetSlot == EEquipmentSlot::None) return false;

    FEquipmentSlotData* Slot = GetSlotByEnum(TargetSlot);
    if (!Slot) return false;

    // If something is already in the slot, return it to inventory first
    if (Slot->bIsOccupied)
    {
        InventoryManager->AddItemToInventory(Slot->ItemData.ItemRowName, 1);
    }

    // Write the new item into the named slot
    Slot->ItemData = *ItemData;
    Slot->ItemData.ItemRowName = ItemRowName;  // store the key so we can return it later
    Slot->bIsOccupied = true;

    // Remove from inventory
    InventoryManager->RemoveItemFromInventory(ItemRowName, 1);

    PushStatBonuses();
    OnEquipmentChanged.Broadcast();
    return true;
}

bool UEquipmentManager::UnequipSlot(EEquipmentSlot Slot)
{
    if (!InventoryManager) return false;
    if (InventoryManager->IsInventoryFull()) return false;

    FEquipmentSlotData* SlotData = GetSlotByEnum(Slot);
    if (!SlotData || !SlotData->bIsOccupied) return false;

    InventoryManager->AddItemToInventory(SlotData->ItemData.ItemRowName, 1);
    SlotData->Clear();

    PushStatBonuses();
    OnEquipmentChanged.Broadcast();
    return true;
}

bool UEquipmentManager::IsSlotOccupied(EEquipmentSlot Slot) const
{
    const FEquipmentSlotData* SlotData = GetSlotByEnum(Slot);
    return SlotData ? SlotData->bIsOccupied : false;
}

FEquipmentSlotData UEquipmentManager::GetSlotData(EEquipmentSlot Slot) const
{
    const FEquipmentSlotData* SlotData = GetSlotByEnum(Slot);
    return SlotData ? *SlotData : FEquipmentSlotData();
}

int32 UEquipmentManager::GetTotalArmourDefense() const
{
    if (!ArmourDataTable) return 0;

    int32 Total = 0;
    const TArray<EEquipmentSlot> ArmourSlots = {
        EEquipmentSlot::Helmet, EEquipmentSlot::Chestplate,
        EEquipmentSlot::Leggings, EEquipmentSlot::Boots, EEquipmentSlot::Gloves
    };

    for (EEquipmentSlot SlotEnum : ArmourSlots)
    {
        const FEquipmentSlotData* Slot = GetSlotByEnum(SlotEnum);
        if (!Slot || !Slot->bIsOccupied) continue;

        const FArmourDataInfo* Data = ArmourDataTable->FindRow<FArmourDataInfo>(
            Slot->ItemData.ItemRowName, TEXT(""));
        if (Data) Total += Data->ArmourDefense;
    }
    return Total;
}

float UEquipmentManager::GetEquippedWeaponAttackDamage() const
{
    if (!WeaponDataTable || !MainHandSlot.bIsOccupied) return 1.0f;

    const FWeaponDataInfo* Data = WeaponDataTable->FindRow<FWeaponDataInfo>(
        MainHandSlot.ItemData.ItemRowName, TEXT(""));
    return Data ? Data->BaseDamage : 1.0f;
}

EWeaponDataType UEquipmentManager::GetEquippedWeaponType() const
{
    if (!WeaponDataTable || !MainHandSlot.bIsOccupied)
        return EWeaponDataType::MiscWeapon;

    const FWeaponDataInfo* Data = WeaponDataTable->FindRow<FWeaponDataInfo>(
        MainHandSlot.ItemData.ItemRowName, TEXT(""));
    return Data ? Data->WeaponType : EWeaponDataType::MiscWeapon;
}

FEquipmentStatSummary UEquipmentManager::GetTotalStatBonuses() const
{
    FEquipmentStatSummary Summary;

    // Armour slots
    const TArray<EEquipmentSlot> ArmourSlots = {
        EEquipmentSlot::Helmet, EEquipmentSlot::Chestplate,
        EEquipmentSlot::Leggings, EEquipmentSlot::Boots, EEquipmentSlot::Gloves
    };
    for (EEquipmentSlot SlotEnum : ArmourSlots)
    {
        const FEquipmentSlotData* Slot = GetSlotByEnum(SlotEnum);
        if (!Slot || !Slot->bIsOccupied || !ArmourDataTable) continue;

        const FArmourDataInfo* Data = ArmourDataTable->FindRow<FArmourDataInfo>(
            Slot->ItemData.ItemRowName, TEXT(""));
        if (!Data) continue;

        Summary.BonusHealth  += Data->BonusHealth;
        Summary.BonusStamina += Data->BonusStamina;
        Summary.BonusMana    += Data->BonusMana;
    }

    // Accessory slots
    const TArray<EEquipmentSlot> AccSlots = {
        EEquipmentSlot::Belt, EEquipmentSlot::HandAcc,
        EEquipmentSlot::HeadAcc, EEquipmentSlot::Charm
    };
    for (EEquipmentSlot SlotEnum : AccSlots)
    {
        const FEquipmentSlotData* Slot = GetSlotByEnum(SlotEnum);
        if (!Slot || !Slot->bIsOccupied || !AccessoryDataTable) continue;

        const FAccessoryItemData* Data = AccessoryDataTable->FindRow<FAccessoryItemData>(
            Slot->ItemData.ItemRowName, TEXT(""));
        if (!Data) continue;

        Summary.BonusHealth  += Data->BonusHealth;
        Summary.BonusStamina += Data->BonusStamina;
        Summary.BonusMana += Data->BonusMana;
    }

    // Weapon
    if (MainHandSlot.bIsOccupied && WeaponDataTable)
    {
        const FWeaponDataInfo* Data = WeaponDataTable->FindRow<FWeaponDataInfo>(
            MainHandSlot.ItemData.ItemRowName, TEXT(""));
        if (Data) Summary.BonusDamage += Data->BaseDamage;
    }

    return Summary;
}

void UEquipmentManager::PushStatBonuses()
{
    if (!StatsComp) return;

    float TotalHP = 0.f;
    float TotalStam = 0.f;
    float TotalMana = 0.f;
    float TotalDamage = 0.f;

    // Armour slots
    const TArray<EEquipmentSlot> ArmourSlots = {
        EEquipmentSlot::Helmet, EEquipmentSlot::Chestplate,
        EEquipmentSlot::Leggings, EEquipmentSlot::Boots, EEquipmentSlot::Gloves
    };
    for (EEquipmentSlot SlotEnum : ArmourSlots)
    {
        const FEquipmentSlotData* Slot = GetSlotByEnum(SlotEnum);
        if (!Slot || !Slot->bIsOccupied || !ArmourDataTable) continue;

        const FArmourDataInfo* Data = ArmourDataTable->FindRow<FArmourDataInfo>(Slot->ItemData.ItemRowName, TEXT(""));
        if (!Data) continue;

        TotalHP += Data->BonusHealth;
        TotalStam += Data->BonusStamina;
        TotalMana += Data->BonusMana;
    }

    // Accessory slots
    const TArray<EEquipmentSlot> AccSlots = {
        EEquipmentSlot::Belt, EEquipmentSlot::HandAcc,
        EEquipmentSlot::HeadAcc, EEquipmentSlot::Charm
    };
    for (EEquipmentSlot SlotEnum : AccSlots)
    {
        const FEquipmentSlotData* Slot = GetSlotByEnum(SlotEnum);
        if (!Slot || !Slot->bIsOccupied || !AccessoryDataTable) continue;

        const FAccessoryItemData* Data = AccessoryDataTable->FindRow<FAccessoryItemData>(Slot->ItemData.ItemRowName, TEXT(""));
        if (!Data) continue;

        TotalHP += Data->BonusHealth;
        TotalStam += Data->BonusStamina;
        TotalMana += Data->BonusMana;
    }
    const TArray<EEquipmentSlot> MainHand = {
        EEquipmentSlot::MainHand
    };
    for (EEquipmentSlot SlotEnum : MainHand)
    {
        const FEquipmentSlotData* Slot = GetSlotByEnum(SlotEnum);
        if (!Slot || !Slot->bIsOccupied || !WeaponDataTable) continue;

        const FWeaponDataInfo* Data = WeaponDataTable->FindRow<FWeaponDataInfo>(Slot->ItemData.ItemRowName, TEXT(""));
        if (!Data) continue;

        TotalDamage += Data->BaseDamage;
    }

    StatsComp->ApplyEquipmentBonuses(TotalHP, TotalStam, TotalMana, TotalDamage);
}
