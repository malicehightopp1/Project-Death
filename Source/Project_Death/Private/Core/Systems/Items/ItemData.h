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
UENUM(BlueprintType)
enum class ERarityType : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None        UMETA(DisplayName = "None"),
	MainHand    UMETA(DisplayName = "Main Hand"),
	Helmet      UMETA(DisplayName = "Helmet"),
	Chestplate  UMETA(DisplayName = "Chestplate"),
	Leggings    UMETA(DisplayName = "Leggings"),
	Boots       UMETA(DisplayName = "Boots"),
	Gloves      UMETA(DisplayName = "Gloves"),
	Belt        UMETA(DisplayName = "Belt"),
	HandAcc     UMETA(DisplayName = "Hand Accessory"),
	HeadAcc     UMETA(DisplayName = "Head Accessory"),
	Charm       UMETA(DisplayName = "Charm")
};

USTRUCT(BlueprintType)
struct FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()
	
	//Base information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FText ItemDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) UTexture2D* ItemTexture;

	//Shop information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 BuyPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 SellPrice;

	//Stacking information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) bool bIsStackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 StackSize;

	//type information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) ERarityType ItemRarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EEquipmentSlot SlotType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemRowName;

	//Item dropping information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) UStaticMesh* ItemMesh;
};

// ========================================================================================================
// ------ Weapon Item Data --------------------------------------------------------------------------------
// ========================================================================================================
USTRUCT(BlueprintType)
struct FEquipmentSlotData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	FItemDataInfo ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	bool bIsOccupied = false;

	void Clear()
	{
		ItemData  = FItemDataInfo();
		bIsOccupied = false;
	}
};
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

	//Item search information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;

	//Weapon specific information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EWeaponDataType WeaponType;
};

// ========================================================================================================
// ------ Armour Item Data --------------------------------------------------------------------------------
// ========================================================================================================
UENUM(BlueprintType)
enum class EArmourType : uint8
{
	Helmet UMETA(DisplayName = "Helmet"),
	Chestplate UMETA(DisplayName = "Chestplate"),
	Leggings UMETA(DisplayName = "Leggings"),
	Boots UMETA(DisplayName = "Boots"),
	Gloves UMETA(DisplayName = "Gloves")
};

UENUM(BlueprintType)
enum class EArmourDefenseType : uint8
{
	Light UMETA(DisplayName = "Light Weight"),
	Medium UMETA(DisplayName = "Medium Weight"),
	Heavy UMETA(DisplayName = "Heavy Weight")
};

USTRUCT(BlueprintType)
struct FArmourDataInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) int32 ArmourDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EArmourDefenseType ArmourDefenseType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EArmourType ArmourType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float BonusHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float BonusStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float BonusMana;
};

// ========================================================================================================
// ------ Consumable Item Data ----------------------------------------------------------------------------
// ========================================================================================================

UENUM(BlueprintType)
enum class EConsumableType : uint8
{
	Throwable UMETA(DisplayName = "Throwable"),
	Consumable UMETA(DisplayName = "Consumable")
};

USTRUCT(BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EConsumableType ConsumableType;
};

// ========================================================================================================
// ------ Accessory Item Data -----------------------------------------------------------------------------
// ========================================================================================================

UENUM(BlueprintType)
enum class EAccessoryType : uint8
{
	BeltAccessory UMETA(DisplayName = "Belt Accessory"),
	HandAccessory UMETA(DisplayName = "Hand Accessory"),
	HeadAccessory UMETA(DisplayName = "Head Accessory"),
	CharmAccessory UMETA(DisplayName = "Charm Accessory")
};
USTRUCT(BlueprintType)
struct FAccessoryItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) EAccessoryType AccessoryType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float BonusHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float BonusStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) float BonusMana;
};

// ========================================================================================================
// ------ Misc Item Data ----------------------------------------------------------------------------------
// ========================================================================================================

USTRUCT(BlueprintType)
struct FMiscItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;
};

// ========================================================================================================
// ------ Quest Item Data ---------------------------------------------------------------------------------
// ========================================================================================================

USTRUCT(BlueprintType)
struct FQuestItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemDataInfo) FName ItemRowName;
};

UCLASS()
class UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
};
