#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "PaperDollWidget.generated.h"

class UEquipmentManager;
class UInventoryManager;

UCLASS()
class UPaperDollWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "PaperDoll")void InitializePaperDoll(UEquipmentManager* InEquipmentManager, UInventoryManager* InInventoryManager);
    
    UFUNCTION(BlueprintCallable, Category = "PaperDoll") void RefreshAllSlots();

    UFUNCTION(BlueprintCallable, Category = "PaperDoll") void RefreshStatbonus();

    UFUNCTION(BlueprintImplementableEvent, Category = "PaperDoll") void UpdateStatbonus(const FEquipmentStatSummary& stats);

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(BlueprintReadOnly, Category = "PaperDoll")
    TObjectPtr<UEquipmentManager> EquipmentManager;

    UPROPERTY(BlueprintReadOnly, Category = "PaperDoll")
    TObjectPtr<UInventoryManager> InventoryManager;

private:
    UFUNCTION()
    void OnEquipmentChanged();
};