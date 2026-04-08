// Brandin stanfield


#include "ItemPickup.h"

#include "Core/Systems/Player/Inventory/InventoryManager.h"

// Sets default values
AItemPickup::AItemPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComp");
	RootComponent = ItemMeshComp;
	
	//setting collision handling
	ItemMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ItemMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	LoaditemData();
}

// Called every frame
void AItemPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPickup::InteractPure(ABaseCharacter* player)
{
	ABaseCharacter* playercharacter = Cast<ABaseCharacter>(player);
	
	if (playercharacter)
	{
		playercharacter->InventoryManagerRef->AddItemToInventory(ItemRowHandle.RowName, ItemQuantity);
		Destroy();
	}
}
FText AItemPickup::GetInteractText_Implementation()
{
	return FText::Format(NSLOCTEXT("Pickup", "PickupText", "Pickup {0}"), FText::FromName(ItemInfo.ItemName)); //getting the name of the item and setting it to the text
}

void AItemPickup::LoaditemData()
{
	if (!ItemRowHandle.DataTable || ItemRowHandle.RowName.IsNone()) //making sure that there is a data table and that there info in the data table
	{
		return;
	}
	FItemDataInfo* FoundRow = ItemRowHandle.DataTable->FindRow<FItemDataInfo>(ItemRowHandle.RowName, TEXT("AItemPickup::LoadItemData"));
	
	if (FoundRow)
	{
		ItemInfo = *FoundRow; //cache the data 
		
		if (ItemInfo.ItemMesh)
		{
			ItemMeshComp->SetStaticMesh(ItemInfo.ItemMesh); //setting the mesh
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No mesh found"))
		}
	}
}

