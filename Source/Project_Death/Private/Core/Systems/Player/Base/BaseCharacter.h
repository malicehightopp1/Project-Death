// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

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
	/// Dodge ----------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Dodge", meta = (Clampmin = "0", ClampMax = "5000")) float DodgeForce = 200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Dodge", meta = (ClampMin = "0", ClampMax = "5.0")) float DodgeDuration = 0.5f;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Dodge") bool bIsDodging = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Dodge") FVector DodgeDirection;
	
	
private:
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Dodge ----------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "Movement") void PlayerDodge();
	UFUNCTION(BlueprintCallable, Category = "Movement") void PlayerDodgeEnd();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Components -----------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UCapsuleComponent* PlayerCapsuleComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UMeshComponent* PlayerMeshComponent; 
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") USpringArmComponent* PlayerBloomComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UCameraComponent* PlayerCameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player | PlayerComp") UCharacterMovementComponent* PlayerMovementComponent;
};
