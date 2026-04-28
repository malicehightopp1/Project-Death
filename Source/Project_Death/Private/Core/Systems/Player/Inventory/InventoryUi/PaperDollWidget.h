#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaperDollWidget.generated.h"

class UEquipmentManager;
class UInventoryManager;

UCLASS()
class UPaperDollWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /** Call this once from the owning HUD/character after the widget is created. */
    UFUNCTION(BlueprintCallable, Category = "PaperDoll")
    void InitializePaperDoll(UEquipmentManager* InEquipmentManager,
                             UInventoryManager* InInventoryManager);

    /** Exposed so the refreshSlots Blueprint function can call it. */
    UFUNCTION(BlueprintCallable, Category = "PaperDoll")
    void RefreshAllSlots();

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