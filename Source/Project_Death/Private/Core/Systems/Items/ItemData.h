// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 
{
	Weapon UMETA(DisplayName = "Weapon"),
	Quest UMETA(DisplayName = "Quest"),
	Armour UMETA(DisplayName = "Armour"),
	Consumable UMETA(DisplayName = "Consumable"),
	Accessory UMETA(DisplayName = "Accesory"),
	Misc UMETA(DisplayName = "Misc"),
	All UMETA(DisplayName = "All")
};
USTRUCT(BlueprintType)
struct FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	//Item rarity *for mini assignment* ccould just be another struct
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FText ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 BuyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 SellPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) UTexture2D* ItemTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) bool bIsStackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 StackSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) UStaticMesh* ItemMesh;
};

// ========================================================================================================
// ------ Weapon Item Data -------------------------------------------------------------------------------
// ========================================================================================================


/*
 * makes another data table for just the weapons *STILL CONNECTED TO THE MAIN BASE DATA TABLE ABOVE*
 */
UENUM(BlueprintType)
enum class EWeaponDataType : uint8
{
	Sword UMETA(DisplayName = "Sword"),
	Axes UMETA(DisplayName = "Axe"),
	Hammer UMETA(DisplayName = "Hammer"),
	Staff UMETA(DisplayName = "Staff"),
	Shield UMETA(DisplayName = "Shield"),
	SpellBook UMETA(DisplayName = "SpellBook"),
	MiscWeapon UMETA(DisplayName = "MiscWeapon")
};
USTRUCT(BlueprintType)
struct FWeaponDataInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EWeaponDataType WeaponType;
};
UCLASS()
class UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
};
