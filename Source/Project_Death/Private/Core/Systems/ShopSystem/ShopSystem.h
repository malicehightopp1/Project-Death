// Brandin stanfield

#pragma once

#include "CoreMinimal.h"
#include "Core/Systems/Interactions/InteractionManager.h"
#include "Core/Systems/Items/ItemData.h"
#include "GameFramework/Actor.h"
#include "ShopSystem.generated.h"

class ABaseCharacter;
enum class EItemType : uint8;
class UInventoryManager;
class UBoxComponent;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class AShopSystem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShopSystem();
	virtual void InteractPure(ABaseCharacter* player); //player interaction
	UFUNCTION(BlueprintCallable, Category = "Shop System | Item Systems") void BuyItem(FName ItemKey, ABaseCharacter* Player);
	UFUNCTION(BlueprintCallable, Category = "Shop System | Item Systems") void SellItem(FName ItemKey, ABaseCharacter* Player);
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional),Category = "Shop System | Data") class UScrollBox* ShopScrollBox;

	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") bool bShopOpen = false;
	UPROPERTY(EditAnywhere, Category = "Shop System | Data") TArray<FName> ShopItemNames;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, Category = "Shop System | UI") UWidgetComponent* WidgetDetectionComponent;

private:
	//UI functions
	UFUNCTION(BlueprintCallable) void RequestDispatch(FName ItemKey);
	UFUNCTION(BlueprintCallable) void RequestSell(FName ItemKey);
	UFUNCTION() void UpdateWidgetUI();

	UFUNCTION(BlueprintCallable) void FillShopItems(UScrollBox* ScrollBox, EItemType NewFilter);

	//Interaction Functions
	UFUNCTION() void SetupShopSystem();
	//Overlap Events functions
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Overlap Variables 
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") bool bPlayerInRange = false;
	
	//Items 
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Data") UDataTable* ItemDataTable;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Data") TSubclassOf<UUserWidget> ShopItemSlots;
	
	//Components
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Components") USphereComponent* SphereDetectionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | Components") UBoxComponent* BoxDetectionComponent;

	//Defaults
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | DefaultValues") float ShopDefaultDetection = 10.0f;

	//UserWidgets
	UPROPERTY(EditDefaultsOnly, Category = "Shop System | UI") TSubclassOf<UUserWidget> ShopsystemUIClass; //reference to the shop widget
	UPROPERTY() UUserWidget* ShopSystemUIInstance; //the actual instance, AKA what we spawn

	//reference
	UPROPERTY() UInventoryManager* inventoryManager;
};