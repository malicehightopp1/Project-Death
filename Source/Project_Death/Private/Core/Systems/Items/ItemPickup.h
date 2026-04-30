#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "GameFramework/Actor.h"
#include "Core/Systems/Interactions/InteractionManager.h"
#include "ItemPickup.generated.h"

class USphereComponent;
class UWidgetComponent;
class ABaseCharacter;

UCLASS()
class AItemPickup : public AActor, public IInteractInterface
{
    GENERATED_BODY()

public:
    AItemPickup();
    // ========================================================================================================
    // ------ initlization function ---------------------------------------------------------------------------
    // ========================================================================================================
    void InitializeItem(UDataTable* DataTable, FName RowName, int32 InQuantity);

protected:
    virtual void BeginPlay() override;

    // ========================================================================================================
    // ------ basic information --------------------------------------------------------------------------------
    // ========================================================================================================
    UPROPERTY(VisibleAnywhere, Category = "Components") UStaticMeshComponent* ItemMeshComp;
    UPROPERTY(EditAnywhere, Category = "Item") int32 ItemQuantity = 1;
    UPROPERTY(BlueprintReadOnly, Category = "Item") FItemDataInfo ItemInfo;


    // ========================================================================================================
    // ------ Item Pickup UI ----------------------------------------------------------------------------------
    // ========================================================================================================
    UPROPERTY(VisibleAnywhere, Category = "Components") UWidgetComponent* InteractWidgetComp;
    UPROPERTY(EditAnywhere, Category = "UI") TSubclassOf<UUserWidget> InteractWidgetClass;
    UPROPERTY(EditAnywhere, Category = "Item") FDataTableRowHandle ItemRowHandle;
    
    // ========================================================================================================
    // ------ Item pickup variables ---------------------------------------------------------------------------
    // ========================================================================================================
    UPROPERTY(VisibleAnywhere, Category = "Components") USphereComponent* ProximitySphere;
    UPROPERTY(EditAnywhere, Category = "Proximity") float ProximityRadius = 50.f;
    UPROPERTY() ABaseCharacter* PlayerInRange = nullptr;

    // ========================================================================================================
    // ------ Overlap management --------------------------------------------------------------------------------
    // ========================================================================================================
    UFUNCTION()void OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()void OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // ========================================================================================================
    // ------ Interface interaction----------------------------------------------------------------------------
    // ========================================================================================================
    virtual void InteractPure(ABaseCharacter* Player) override;
    virtual FText GetInteractText_Implementation();

    // ========================================================================================================
    // ------ helping functions -------------------------------------------------------------------------------
    // ========================================================================================================
    void LoadItemData();
    void ShowInteractPrompt(bool bShow);
    
};