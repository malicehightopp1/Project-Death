#include "PaperDollWidget.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"

void UPaperDollWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Grab the player and pull components directly — no need for external InitializePaperDoll call
	APlayerController* PC = GetOwningPlayer();
	if (!PC) 
	{
		UE_LOG(LogTemp, Warning, TEXT("PaperDollWidget: No owning PlayerController"));
		return;
	}

	APawn* Pawn = PC->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PaperDollWidget: No pawn found"));
		return;
	}

	EquipmentManager = Pawn->FindComponentByClass<UEquipmentManager>();
	InventoryManager = Pawn->FindComponentByClass<UInventoryManager>();

	UE_LOG(LogTemp, Warning, TEXT("PaperDollWidget: EqManager=%s InvManager=%s"),
		EquipmentManager ? TEXT("FOUND") : TEXT("NULL"),
		InventoryManager ? TEXT("FOUND") : TEXT("NULL"));

	if (!EquipmentManager) return;

	EquipmentManager->OnEquipmentChanged.AddDynamic(this, &UPaperDollWidget::OnEquipmentChanged);

	RefreshAllSlots();
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
	RefreshStatbonus();
}

void UPaperDollWidget::RefreshStatbonus()
{
	if (!EquipmentManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("PaperDollWidget: EquipmentManager is NULL — was InitializePaperDoll called?"));
		return;
	}

	FEquipmentStatSummary Stats = EquipmentManager->GetTotalStatBonuses();

	UE_LOG(LogTemp, Display, TEXT("PaperDollWidget: HP=%.1f SP=%.1f MP=%.1f DMG=%.1f"),
		Stats.BonusHealth, Stats.BonusStamina, Stats.BonusMana, Stats.BonusDamage);

	UpdateStatbonus(Stats);
}
