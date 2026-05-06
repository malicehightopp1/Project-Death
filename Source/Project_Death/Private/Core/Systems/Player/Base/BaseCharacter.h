// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Core/Systems/Player/Inventory/EquipmentManager.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCharacterStatsComp;
class UPlayerWidget;
struct FInputActionValue;
class UInventoryManager;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UAnimInstance;

UCLASS()
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// References -----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment") class UEquipmentManager* EquipmentManager;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Ref") UInventoryManager* InventoryManagerRef;	
	UPROPERTY(BlueprintReadOnly) bool bIsAttacking = false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Ui ------------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY() UUserWidget* PlayerWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Player | UI") TSubclassOf<UPlayerWidget> PlayerWidgetClass;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Dodge ----------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(BlueprintReadOnly, Category = "Player | Movement | Dodge") bool bIsDodging = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player | Movement | Dodge") FVector DodgeDirection;
private:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Input Action Functions -----------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION() void Interact(const FInputActionValue& Value);
	UFUNCTION() void InventoryToggle(const FInputActionValue& Value);
	UFUNCTION() void Attack(const FInputActionValue& Value);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Input actions --------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* InteractionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* InventoryAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* PauseAction;
	UPROPERTY(EditDefaultsOnly, Category = "Player | Player Input") UInputAction* AttackAction;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Dodge ----------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "Movement") void PlayerDodge();
	UFUNCTION(BlueprintCallable, Category = "Movement") void PlayerDodgeStart();
	UFUNCTION(BlueprintCallable, Category = "Movement") void PlayerDodgeEnd();
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement") float SprintSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement") float WalkSpeed   = 300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sprint") UCharacterStatsComp* CharacterStats; 
	
	UFUNCTION(BlueprintCallable, Category = "Movement") void OnSprintChanged (bool bSprinting);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Components -----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UCapsuleComponent* PlayerCapsuleComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UMeshComponent* PlayerMeshComponent; 
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") USpringArmComponent* PlayerBloomComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UCameraComponent* PlayerCameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UCharacterMovementComponent* PlayerMovementComponent;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Attack ---------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(blueprintCallable, Category = "Player | PlayerAttack") void PerformSphereAttack();
	UFUNCTION(BlueprintCallable) void OnAttackEnd();
	UFUNCTION(BlueprintCallable) void OnAttackHitFrame();
	UFUNCTION() void LockRotation(bool bLock);
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerAttack") float AttackRadius = 80.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerAttack") float AttackDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerAttack") float AttackRange = 70.0f;

	TArray<TWeakObjectPtr<AActor>> HitActorsThisSwing;  //TWeakObjectPtr so it doesnt crash 
	
	UPROPERTY(EditAnywhere, Category = "Player | PlayerTakeDamage")UAnimMontage* HitReactMontage;

	UFUNCTION()void PlayHitReact();
	UPROPERTY(EditAnywhere, Category = "Combat") TSubclassOf<UCameraShakeBase> HitCameraShake;

	UPROPERTY(EditAnywhere, Category = "Combat") TArray<UAnimMontage*> ComboMontages;
	UPROPERTY() int32 ComboCount = 0;
	UPROPERTY() bool bComboQueued = false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// interaction ----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Player | Interaction") float InteractionRange = 150.f;
};
