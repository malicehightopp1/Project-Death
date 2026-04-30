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
    void InitializeItem(UDataTable* DataTable, FName RowName, int32 InQuantity);

protected:
    virtual void BeginPlay() override;

    // --- Components ---
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* ItemMeshComp;

    // Proximity detection sphere
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* ProximitySphere;

    // World-space interaction prompt widget
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UWidgetComponent* InteractWidgetComp;

    // --- Data ---
    UPROPERTY(EditAnywhere, Category = "Item")
    FDataTableRowHandle ItemRowHandle;

    UPROPERTY(EditAnywhere, Category = "Item")
    int32 ItemQuantity = 1;

    UPROPERTY(BlueprintReadOnly, Category = "Item")
    FItemDataInfo ItemInfo;

    // The widget class to spawn (make a UUserWidget BP for this)
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> InteractWidgetClass;

    UPROPERTY(EditAnywhere, Category = "Proximity")
    float ProximityRadius = 50.f;

    // Tracks if a player is currently in range
    UPROPERTY()
    ABaseCharacter* PlayerInRange = nullptr;

    // --- Overlap Callbacks ---
    UFUNCTION()
    void OnProximityBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnProximityEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // --- IInteractInterface ---
    virtual void InteractPure(ABaseCharacter* Player) override;
    virtual FText GetInteractText_Implementation() override;

    // --- Helpers ---
    void LoadItemData();
    void ShowInteractPrompt(bool bShow);
    
};