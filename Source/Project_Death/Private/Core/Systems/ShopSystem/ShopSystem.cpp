// Brandin stanfield


#include "ShopSystem.h"

#include "Components/BoxComponent.h"
#include "Components/ScrollBox.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Items/ItemData.h"
#include "Core/Systems/Player/Base/BaseCharacter.h"
#include "Core/Systems/Player/Base/PlayerCurrency/CurrencyManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"
#include "Kismet/GameplayStatics.h"

/*
 * Handles input of shop system, Ui on shop, and buying/selling of items
 * Reads off Data Table of items to set values on the widgets
 * 
 * TODO Buying and selling items, along with rework to bring up Reforge UI.
 *
 * - button to call reforgeItem - bind function to button
 * - bring up Ui when interaction happens - easy part
 */
AShopSystem::AShopSystem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//Player detection for UI
	SphereDetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Player Detection"));
	SphereDetectionComponent->SetupAttachment(RootComponent);
	
	//Setting overlap event on
	SphereDetectionComponent->SetGenerateOverlapEvents(true);
	SphereDetectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereDetectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	//default values - Size
	ShopDefaultDetection = 300.0f;
	SphereDetectionComponent->SetSphereRadius(ShopDefaultDetection); //default sphere radius
	
	//NPC Hit box
	BoxDetectionComponent = CreateDefaultSubobject<UBoxComponent>("Interaction Box Component");
	BoxDetectionComponent->SetupAttachment(SphereDetectionComponent);
	BoxDetectionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	//Creating widget
	WidgetDetectionComponent = CreateDefaultSubobject<UWidgetComponent>("Player Interaction Widget");
	
	WidgetDetectionComponent->SetupAttachment(SphereDetectionComponent);
	WidgetDetectionComponent->SetVisibility(false);
	WidgetDetectionComponent->SetWidgetSpace(EWidgetSpace::Screen);
}
void AShopSystem::BeginPlay()
{
	Super::BeginPlay();
	SphereDetectionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShopSystem::OnOverlapBegin); //binding sphere to begin overlap
	SphereDetectionComponent->OnComponentEndOverlap.AddDynamic(this, &AShopSystem::OnOverlapEnd); //binding sphere to end overlap
	
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn(); //getting player pawn to grab the inventory system
	if (PlayerPawn)
	{
		inventoryManager = PlayerPawn->FindComponentByClass<UInventoryManager>();
	}
}

void AShopSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Shop Mechanics / interaction with shop systems
void AShopSystem::InteractPure(ABaseCharacter* player) //interaction
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ABaseCharacter* character = Cast<ABaseCharacter>(PC->GetPawn());
	
	UInventoryManager* inventory = character->FindComponentByClass<UInventoryManager>();
	if (bPlayerInRange && bShopOpen == false) // turn on the UI
	{
		SetupShopSystem();
		bShopOpen = true;
		if (PC) //locking movement and turning cursor on 
		{
			PC->SetShowMouseCursor(true);
			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);
		}
	}
	else if (bShopOpen == true) //if shop is active turn it off
	{
		if (ShopSystemUIInstance)
		{
			ShopSystemUIInstance->RemoveFromParent();
		}
		bShopOpen = false;
		if (PC)
		{
			PC->SetIgnoreMoveInput(false);
			PC->SetIgnoreLookInput(false);
			PC->bShowMouseCursor = false;
		}
	}
}

//actually buying the item and using the players current currency
void AShopSystem::BuyItem(FName ItemKey, ABaseCharacter* Player) //TODO buying and selling items from the shop. Right now just have it print out the item youre trying to buy and take currency 
{
	if (!ItemDataTable || !Player) return;
	if (!inventoryManager) return;

	if (inventoryManager->IsInventoryFull())
	{
		UE_LOG(LogTemp, Display, TEXT("Inventory Full"));
		return;
	}

	FItemDataInfo* ItemToBuy = ItemDataTable->FindRow<FItemDataInfo>(ItemKey, "");
	if (!ItemToBuy) return;

	UCurrencyManager* Currency = Player->GetCurrencySystem();
	if (!Currency) return;

	if (Currency->GetPlayerCurrentCurrency() >= ItemToBuy->BuyPrice)
	{
		Currency->OnCurrencyChange(-ItemToBuy->BuyPrice);
		inventoryManager->AddItemToInventory(ItemKey, 1);
		inventoryManager->RefreshInventory();
	}
}

//selling the item and giving the player money back
void AShopSystem::SellItem(FName ItemKey, ABaseCharacter* Player)
{
    if (!ItemDataTable || !Player) return;
    if (!inventoryManager) return;

    FItemDataInfo* ItemToBuy = ItemDataTable->FindRow<FItemDataInfo>(ItemKey, "");
    if (!ItemToBuy) return;

    UCurrencyManager* Currency = Player->GetCurrencySystem();
    if (!Currency) return;

    if (inventoryManager->Hasitem(ItemKey, 1))
    {
        Currency->OnCurrencyChange(ItemToBuy->SellPrice);
        inventoryManager->RemoveItemFromInventory(ItemKey, 1);
        inventoryManager->RefreshInventory();
    }
}

//delegate to buy items - binding the on click
void AShopSystem::RequestDispatch(FName ItemKey) //for buying items
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (Player)
	{
		BuyItem(ItemKey, Player);
	}
}

void AShopSystem::RequestSell(FName ItemKey)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		SellItem(ItemKey, Player);
	}
}
#pragma endregion 

#pragma region Updating UI
void AShopSystem::UpdateWidgetUI()
{
	bool bRangeCheck = bPlayerInRange;
	
	if (WidgetDetectionComponent)
	{
		WidgetDetectionComponent->SetVisibility(bRangeCheck);
	}
}

void AShopSystem::FillShopItems(UScrollBox* ScrollBox, EItemType NewFilter)
{
	if (!ScrollBox || !ItemDataTable) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ScrollBox->ClearChildren();
	for (const FName& RowName : ShopItemNames) //looping through all items to check the Enum types
	{
		FItemDataInfo* ItemData = ItemDataTable->FindRow<FItemDataInfo>(RowName, TEXT(""));
		if (!ItemData) continue;

		// Filter check - skip if not matching active tab
		if (NewFilter != EItemType::All && ItemData->ItemType != NewFilter) //checking if its the active filter
			continue;

		if (ShopItemSlots)
		{
			UUserWidget* NewSlot = CreateWidget<UUserWidget>(PC, ShopItemSlots);
			if (FProperty* Prop = NewSlot->GetClass()->FindPropertyByName(TEXT("ItemKey"))) //setting the values for the slots
			{
				FNameProperty* NameProp = CastField<FNameProperty>(Prop);
				NameProp->SetPropertyValue_InContainer(NewSlot, RowName);
			}

			FMulticastDelegateProperty* DelegateBuyRequest = FindFProperty<FMulticastDelegateProperty>(NewSlot->GetClass(), TEXT("RequestDispatch"));
			FMulticastDelegateProperty* DelegateSellRequest = FindFProperty<FMulticastDelegateProperty>(NewSlot->GetClass(), TEXT("RequestSell"));

			if (DelegateBuyRequest)
			{
				FScriptDelegate DelegateBuy;
				DelegateBuy.BindUFunction(this, FName("RequestDispatch"));
				DelegateBuyRequest->AddDelegate(DelegateBuy, NewSlot);
			}
			if (DelegateSellRequest)
			{
				FScriptDelegate DelegateSell;
				DelegateSell.BindUFunction(this, FName("RequestSell"));
				DelegateSellRequest->AddDelegate(DelegateSell, NewSlot);
			}

			ScrollBox->AddChild(NewSlot);
		}
	}
}

void AShopSystem::SetupShopSystem()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (!ShopSystemUIInstance && ShopsystemUIClass) //create the widget only if it doesnt exist
		{
			ShopSystemUIInstance = CreateWidget<UUserWidget>(PC, ShopsystemUIClass);
		}
		if (ShopSystemUIInstance) //if you do an "else if" statement it wont work
		{
			ShopSystemUIInstance->AddToViewport();
			bShopOpen = true;

			//grabbing all scroll boxes
			UScrollBox* AllBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("AllScrollBox")));
			UScrollBox* WeaponBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("WeaponScrollBox")));
			UScrollBox* ArmourBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("ArmourScrollBox")));
			UScrollBox* ConsumableBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("ConsumableScrollBox")));
			UScrollBox* AccesoryBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("AccesoryScrollBox")));
			UScrollBox* MiscBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("MiscScrollBox")));
			UScrollBox* QuestBox = Cast<UScrollBox>(ShopSystemUIInstance->GetWidgetFromName(TEXT("QuestScrollBox")));
			
			//filling each scroll box with boxes
			FillShopItems(AllBox, EItemType::All);
			FillShopItems(WeaponBox, EItemType::Weapon);
			FillShopItems(QuestBox, EItemType::Quest);
			FillShopItems(ArmourBox, EItemType::Armour);
			FillShopItems(ConsumableBox, EItemType::Consumable);
			FillShopItems(AccesoryBox, EItemType::Accessory);
			FillShopItems(MiscBox, EItemType::Misc);
		}
}
#pragma endregion

#pragma region OverlapEvents
void AShopSystem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		bPlayerInRange = true;
		UpdateWidgetUI();
	}
}
void AShopSystem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		bPlayerInRange = false;
		UpdateWidgetUI();
	}
}
#pragma endregion