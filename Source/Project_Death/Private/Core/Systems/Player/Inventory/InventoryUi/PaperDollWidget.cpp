#include "PaperDollWidget.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"

void UPaperDollWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPaperDollWidget::NativeDestruct()
{
	// Clean up delegate so we don't fire into a dead widget
	if (EquipmentManager)
		EquipmentManager->OnEquipmentChanged.RemoveDynamic(
			this, &UPaperDollWidget::OnEquipmentChanged);

	Super::NativeDestruct();
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
	// Intentionally left minimal — the heavy lifting is in
	// the Blueprint refreshSlots function which reads the named
	// slot properties (HeadSlot, ChestSlot, etc.) directly and
	// calls PopulateSpot on each widget. This function exists
	// so Blueprint can call it and so the delegate can trigger it.
	//
	// If you move refreshSlots to C++ later, implement it here.
}