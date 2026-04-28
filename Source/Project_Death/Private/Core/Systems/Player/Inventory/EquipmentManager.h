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
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData HeadSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData ChestSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData LegsSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData BootsSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData GlovesSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData MainHandSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData BeltSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData HandAccSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData HeadAccSlot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment|Slots")FEquipmentSlotData CharmSlot;

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
    
    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FOnEquipmentChanged OnEquipmentChanged;
    
    UFUNCTION(BlueprintCallable, Category = "Equipment")bool EquipItem(FName ItemRowName);
    
    UFUNCTION(BlueprintCallable, Category = "Equipment")bool UnequipSlot(EEquipmentSlot Slot);

    UFUNCTION(BlueprintPure, Category = "Equipment")bool IsSlotOccupied(EEquipmentSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Equipment")FEquipmentSlotData GetSlotData(EEquipmentSlot Slot) const;
    
    UFUNCTION(BlueprintPure, Category = "Equipment")EEquipmentSlot GetSlotTypeForItem(FName ItemRowName) const;
    
    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")int32 GetTotalArmourDefense() const;

    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")float GetEquippedWeaponAttackDamage() const;

    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")EWeaponDataType GetEquippedWeaponType() const;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()UInventoryManager* InventoryManager;

    UPROPERTY()UCharacterStatsComp* StatsComp;
    
    FEquipmentSlotData* GetSlotByEnum(EEquipmentSlot Slot);
    const FEquipmentSlotData* GetSlotByEnum(EEquipmentSlot Slot) const;
    
    void PushStatBonuses();
};