#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Systems/Items/ItemData.h"
#include "EquipmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UEquipmentManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UEquipmentManager();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")TMap<EEquipmentSlot, FName> EquippedItems;

    UPROPERTY() class UInventoryManager* InventoryManager;
    UPROPERTY() class UCharacterStatsComp* StatsComp;

    //data tables
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")UDataTable* ItemDataTable;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")UDataTable* WeaponDataTable;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")UDataTable* ArmourDataTable;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")UDataTable* AccessoryDataTable;

    UPROPERTY(BlueprintAssignable, Category = "Equipment")FOnEquipmentChanged OnEquipmentChanged;

    UFUNCTION(BlueprintCallable, Category = "Equipment")bool EquipItem(FName ItemRowName);
    UFUNCTION(BlueprintCallable, Category = "Equipment")bool UnequipItem(EEquipmentSlot Slot);
    UFUNCTION(BlueprintCallable, Category = "Equipment")bool IsSlotOccupied(EEquipmentSlot Slot);
    UFUNCTION(BlueprintCallable, Category = "Equipment")FName GetItemInSlot(EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")int32 GetTotalArmourDefense();
    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")float GetEquippedWeaponAttackSpeed();
    UFUNCTION(BlueprintCallable, Category = "Equipment|Stats")EWeaponDataType GetEquippedWeaponType();
protected:
    virtual void BeginPlay() override;

private:
    EEquipmentSlot GetSlotForItem(FName ItemRowName);

    void PushAccessoryBonusesToStats();
};