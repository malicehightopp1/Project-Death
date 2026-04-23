// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Systems/Items/ItemData.h"
#include "InventoryManager.generated.h"


class UPaperDollWidget;
class AItemPickup;
//Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

USTRUCT(BlueprintType)
struct FInventorySlot //definition of what a inventory slot is
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsEmpty;
	
	FInventorySlot() //setting defaults for slots *keeping them empty NOT SAVING*
	{
		ItemRowName = NAME_None;
		Quantity = 0;
		bIsEmpty = true;
	}
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryManager();
	
	// ========================================================================================================
	// ------ Functions ---------------------------------------------------------------------------------------
	// ========================================================================================================
	
	UFUNCTION(BlueprintCallable, Category="Components|Inventory") bool AddItemToInventory(FName ItemRowName, int32 Quantity); //function for adding items using the struct of inventory slot
	UFUNCTION(blueprintCallable, Category="Components|Inventory") bool RemoveItemFromInventory(FName ItemRowName, int32 Quantity); // Function for removing items from inventory
	UFUNCTION(BlueprintCallable, Category="Components|Inventory") bool Hasitem(FName ItemRowName, int32 Quantity = 1); //checking if the player has the item *FOR STACKING*
	UFUNCTION(BLueprintCallable, Category="Components|Inventory") int32 GetItemQuantity(FName ItemRowName);
	UFUNCTION(BlueprintCallable, Category="Components|Inventory") bool IsInventoryFull();
	UFUNCTION(BlueprintCallable, Category="Components|Inventory") bool ItemSwap(int32 SlotIndexA, int32 SlotIndexB);
	UFUNCTION(BlueprintCallable, Category="Components|Inventory") void DropItem(FName ItemRowName, int32 Quantity, FVector Spawnlocation);
	FWeaponDataInfo* GetWeaponData(FName ItemRowName);
	FArmourDataInfo* GetArmourData(FName ItemRowName);
	
	// ========================================================================================================
	// ------ Inventory UI Turn On ----------------------------------------------------------------------------
	// ========================================================================================================
	UFUNCTION() void Inventory();
	UPROPERTY() UPaperDollWidget* PaperDollInstance;
	UPROPERTY(EditDefaultsOnly, Category = "Player") TSubclassOf<UUserWidget> PaperDollClass; //actual class reference
	
	UPROPERTY() UUserWidget* InventoryWidgetInstance; //instance of the class  for creating
	UPROPERTY(EditDefaultsOnly, Category = "Player") TSubclassOf<UUserWidget> InventoryWidgetClass; //actual class reference
	UPROPERTY(EditDefaultsOnly, Category = "Player") bool bIsInventoryOpen = false;
	
	//UI Functions 
	UFUNCTION(BlueprintPure, Category="Components|Inventory") TArray<FInventorySlot> GetAllSLots() const {return InventorySlots;}
	UFUNCTION(BlueprintPure, Category="Components|Inventory") FInventorySlot GetSlotAtIndex(int32 SlotIndex) const;
	UFUNCTION(BlueprintPure, Category="Components|Inventory") int32 GetMaxSlotIndex() const {return MaxSlots; }
	UFUNCTION(BlueprintCallable, Category="Components|Inventory") void RefreshInventory();
	
	// ========================================================================================================
	// ------ Config for inventory ----------------------------------------------------------------------------
	// ========================================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Config") int32 MaxSlots = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Config") UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Config") UDataTable* WeaponDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Config") UDataTable* ArmourDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Config") TArray<FInventorySlot> InventorySlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning") TSubclassOf<AItemPickup> ItemPickupClass;
	// ========================================================================================================
	// ------ Delegates ---------------------------------------------------------------------------------------
	// ========================================================================================================
	
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Update") FOnInventoryChanged OnInventoryChanged; //for updating inventory

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	int32 FindSlotIndex(FName ItemRowName);
	int32 FindEmptySlotIndex();
};
