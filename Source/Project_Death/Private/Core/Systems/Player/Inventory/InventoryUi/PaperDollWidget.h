#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaperDollWidget.generated.h"

UCLASS()
class UPaperDollWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Paper Doll")
    void InitializePaperDoll(class UEquipmentManager* InEquipmentManager, class UInventoryManager* InInventoryManager);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY() class UEquipmentManager* EquipmentManager;
    UPROPERTY() class UInventoryManager* InventoryManager;

    UFUNCTION()
    void OnEquipmentChanged();

    // Refreshes all slot widgets when equipment changes
    void RefreshAllSlots();
};