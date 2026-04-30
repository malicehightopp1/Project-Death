#include "ItemPickup.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Core/Systems/Player/Base/BaseCharacter.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"

AItemPickup::AItemPickup()
{
    PrimaryActorTick.bCanEverTick = false;

    // Mesh
    ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComp");
    RootComponent = ItemMeshComp;
    ItemMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ItemMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

    // Proximity sphere
    ProximitySphere = CreateDefaultSubobject<USphereComponent>("ProximitySphere");
    ProximitySphere->SetupAttachment(RootComponent);
    ProximitySphere->SetSphereRadius(ProximityRadius);
    ProximitySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ProximitySphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    ProximitySphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // World-space widget for the prompt
    InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>("InteractWidgetComp");
    InteractWidgetComp->SetupAttachment(RootComponent);
    InteractWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
    InteractWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 25.f)); // float above item
    InteractWidgetComp->SetVisibility(false); // hidden until player is near
}

void AItemPickup::InitializeItem(UDataTable* DataTable, FName RowName, int32 InQuantity)
{
    ItemRowHandle.DataTable = DataTable;
    ItemRowHandle.RowName = RowName;
    ItemQuantity = InQuantity;
    LoadItemData();
}

void AItemPickup::BeginPlay()
{
    Super::BeginPlay();

    if (ItemRowHandle.DataTable && !ItemRowHandle.RowName.IsNone())
    {
        LoadItemData();
    }

    // Bind overlap events
    ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnProximityBeginOverlap);
    ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AItemPickup::OnProximityEndOverlap);

    // Spawn and assign the widget if a class is set
    if (InteractWidgetClass)
    {
        InteractWidgetComp->SetWidgetClass(InteractWidgetClass);
    }
}

// --- Overlap ---

void AItemPickup::OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
    if (Player)
    {
        PlayerInRange = Player;
        ShowInteractPrompt(true);
    }
}

void AItemPickup::OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
    if (Player && Player == PlayerInRange)
    {
        PlayerInRange = nullptr;
        ShowInteractPrompt(false);
    }
}

// --- IInteractInterface ---

void AItemPickup::InteractPure(ABaseCharacter* Player)
{
    // Guard: only allow pickup if this player is actually in range
    if (!Player || Player != PlayerInRange) return;

    if (Player->InventoryManagerRef)
    {
        Player->InventoryManagerRef->AddItemToInventory(ItemRowHandle.RowName, ItemQuantity);
    }

    Destroy();
}

FText AItemPickup::GetInteractText_Implementation()
{
    return FText::Format(
        NSLOCTEXT("Pickup", "PickupText", "Pick up {0}"),
        FText::FromName(ItemInfo.ItemName)
    );
}

// --- Helpers ---

void AItemPickup::ShowInteractPrompt(bool bShow)
{
    if (InteractWidgetComp)
    {
        InteractWidgetComp->SetVisibility(bShow);
    }
}

void AItemPickup::LoadItemData()
{
    if (!ItemRowHandle.DataTable || ItemRowHandle.RowName.IsNone()) return;

    FItemDataInfo* FoundRow = ItemRowHandle.DataTable->FindRow<FItemDataInfo>(
        ItemRowHandle.RowName, TEXT("AItemPickup::LoadItemData"));

    if (FoundRow)
    {
        ItemInfo = *FoundRow;

        if (ItemInfo.ItemMesh)
        {
            ItemMeshComp->SetStaticMesh(ItemInfo.ItemMesh);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemPickup: No mesh found for %s"), *ItemRowHandle.RowName.ToString());
        }
    }
}