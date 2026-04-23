#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Systems/Items/ItemData.h"
#include "EquipmentSlotWidget.generated.h"

UCLASS()
class UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Set in the paper doll widget when it creates each slot
	UPROPERTY(BlueprintReadWrite, Category = "Equipment Slot")EEquipmentSlot SlotType = EEquipmentSlot::None;

	// Reference back to the equipment manager
	UPROPERTY(BlueprintReadWrite, Category = "Equipment Slot")class UEquipmentManager* EquipmentManager;

	// Reference to inventory manager for item data lookups
	UPROPERTY(BlueprintReadWrite, Category = "Equipment Slot")class UInventoryManager* InventoryManager;

	// Called by the paper doll when equipment changes
	// Updates the icon and empty state
	UFUNCTION(BlueprintCallable, Category = "Equipment Slot")void RefreshSlot();

	// Called when the player clicks this slot
	UFUNCTION(BlueprintCallable, Category = "Equipment Slot")void OnSlotClicked();

	// Exposed to UMG so Blueprint can drive visuals
	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot")UTexture2D* ItemIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot")bool bIsOccupied = false;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment Slot")FText ItemName;

protected:
	// Bind your Image and Button in UMG to these
	UPROPERTY(meta = (BindWidget))class UImage* SlotIcon;

	UPROPERTY(meta = (BindWidget))class UButton* SlotButton;
	
	virtual void NativeConstruct() override;
};