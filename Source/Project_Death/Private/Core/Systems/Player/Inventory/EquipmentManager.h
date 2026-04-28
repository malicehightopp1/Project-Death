#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Systems/Items/ItemData.h"
#include "EquipmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

class UInventoryManager;
class UCharacterStatsComp;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UEquipmentManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UEquipmentManager();

    // ----------------------------------------------------------------
    // Named equipment slots — each one exposed so Blueprint can read
    // them directly for UI display, stat checks, etc.
    // ----------------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData HeadSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData ChestSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData LegsSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData BootsSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData GlovesSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData MainHandSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData BeltSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData HandAccSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData HeadAccSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")
    FEquipmentSlotData CharmSlot;

    // ----------------------------------------------------------------
    // Data tables
    // ----------------------------------------------------------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    UDataTable* WeaponDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    UDataTable* ArmourDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    UDataTable* AccessoryDataTable;

    // ----------------------------------------------------------------
    // Delegate — broadcast whenever any slot changes
    // ----------------------------------------------------------------
    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FOnEquipmentChanged OnEquipmentChanged;

    // ----------------------------------------------------------------
    // Core API
    // ----------------------------------------------------------------

    /** Looks up ItemRowName in ItemDataTable, determines the correct
     *  slot automatically, swaps out any existing item, equips the new one. */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool EquipItem(FName ItemRowName);

    /** Unequips whatever is in the given slot, returns it to inventory. */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool UnequipSlot(EEquipmentSlot Slot);

    /** Returns true if the named slot is occupied. */
    UFUNCTION(BlueprintPure, Category = "Equipment")
    bool IsSlotOccupied(EEquipmentSlot Slot) const;

    /** Returns a copy of the slot data for the given slot. */
    UFUNCTION(BlueprintPure, Category = "Equipment")
    FEquipmentSlotData GetSlotData(EEquipmentSlot Slot) const;

    /** Returns the slot type an item belongs to (NAME_None → EEquipmentSlot::None). */
    UFUNCTION(BlueprintPure, Category = "Equipment")
    EEquipmentSlot GetSlotTypeForItem(FName ItemRowName) const;

    // ----------------------------------------------------------------
    // Stat helpers — keep these so combat code has a clean API
    // ----------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")
    int32 GetTotalArmourDefense() const;

    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")
    float GetEquippedWeaponAttackSpeed() const;

    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")
    EWeaponDataType GetEquippedWeaponType() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UInventoryManager* InventoryManager;

    UPROPERTY()
    UCharacterStatsComp* StatsComp;

    // Returns a pointer to the named slot property for the given enum value.
    // Returns nullptr if the slot enum isn't mapped.
    FEquipmentSlotData* GetSlotByEnum(EEquipmentSlot Slot);
    const FEquipmentSlotData* GetSlotByEnum(EEquipmentSlot Slot) const;

    void PushStatBonuses();
};