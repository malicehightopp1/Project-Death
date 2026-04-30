// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/Systems/Enemies/Enemy Stats/EnemyBaseStatsComp.h"
#include "Core/Systems/Interactions/InteractionManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"
#include "Core/Systems/Player/PlayerUI/PlayerWidget.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerStats/CharacterStatsComp.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Default component setups
	PlayerMeshComponent = GetMesh();
	PlayerCapsuleComponent = GetCapsuleComponent();
	
	//So root motion will work for dodge
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	
	//Setting player controller rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//Setting players movement to match rotation to movement not camera
	PlayerMovementComponent = GetCharacterMovement();
	PlayerMovementComponent->bOrientRotationToMovement = true;
	PlayerMovementComponent->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	//Spring arm defualt values
	PlayerBloomComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerBloomComponent"));
	PlayerBloomComponent->SetupAttachment(RootComponent);
	PlayerBloomComponent->TargetArmLength = 450.0f; //default length from player
	PlayerBloomComponent->SocketOffset.Z = 100.0f; //default length above player
	
	//Default Bloom rotations for rotating around player
	PlayerBloomComponent->bUsePawnControlRotation = true;
	PlayerBloomComponent->bInheritPitch = true;
	PlayerBloomComponent->bInheritYaw = true;
	PlayerBloomComponent->bInheritRoll = false; //needs to be false
	
	//Camera default values
	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCameraComponent->SetupAttachment(PlayerBloomComponent);
	PlayerCameraComponent->bUsePawnControlRotation = false; //the bloom handles this
	
	EquipmentManager = CreateDefaultSubobject<UEquipmentManager>(TEXT("EquipmentManager")); //creating the equipment manager
}

#pragma region BasicSetup
// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidget) //settng player widget Will make a change to a UserWidget C++ class
	{
		PlayerWidget = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidgetClass);
		PlayerWidget->AddToViewport();
	}
	InventoryManagerRef = FindComponentByClass<UInventoryManager>(); //grabbing the inventory manager off the player
	CharacterStats = FindComponentByClass<UCharacterStatsComp>();
	CharacterStats->OnSprintChanged.AddDynamic(this, &ABaseCharacter::OnSprintChanged);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CharacterStats)
	{
		GetCharacterMovement()->MaxWalkSpeed = CharacterStats->bIsSprinting ? SprintSpeed : WalkSpeed;
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &ABaseCharacter::Interact);

		//Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ABaseCharacter::InventoryToggle);
		
		//attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABaseCharacter::Attack);
	}
}

#pragma endregion
#pragma region interactions
void ABaseCharacter::Interact(const FInputActionValue& Value) //setting the definition for interaction *TODO Add a ui that pops up over items the player walks up to*
{
	UE_LOG(LogTemp, Display, TEXT("Interact called"));

	// Find all actors overlapping a sphere around the player
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(InteractionRange), // add "float InteractRadius = 150.f" to your header
		Params
	);

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor) continue;

		if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			Cast<IInteractInterface>(HitActor)->InteractPure(this);
			return; 
		}
	}
}

void ABaseCharacter::InventoryToggle(const FInputActionValue& Value) //toggling the inventory off and on
{
	if (!InventoryManagerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Inventory Ref"))
		return;
	}
	InventoryManagerRef->Inventory(); //turning inventory on from inventory manager
}
#pragma endregion
#pragma region Dodging
void ABaseCharacter::PlayerDodge() //dodge for player
{
	if (InventoryManagerRef && InventoryManagerRef->bIsInventoryOpen) return; //dont allow when inventory open
	if (bIsDodging) return; //don't allow player to dodge if already dodging

	bIsDodging = true;

	// Get the direction based on player input
	// Uses last movement input so dodge goes the direction you're moving
	DodgeDirection = GetLastMovementInputVector();
	
	if (DodgeDirection.IsNearlyZero()) //if there no input dodge backwards
	{
		DodgeDirection = -GetActorForwardVector();
	}

	DodgeDirection.Z = 0.f; // keep it horizontal
	DodgeDirection.Normalize();

	//LaunchCharacter(DodgeDirection * DodgeForce, true, true);
	SetActorRotation(DodgeDirection.ToOrientationRotator());
}

void ABaseCharacter::PlayerDodgeEnd() //resetting the dodge *use this for anim notify*
{
	bIsDodging = false;
	DodgeDirection = FVector::ZeroVector;
}
#pragma endregion
#pragma region Sprinting
void ABaseCharacter::OnSprintChanged(bool bSprinting)
{
	GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
}
#pragma endregion
#pragma region Attack
void ABaseCharacter::Attack(const FInputActionValue& Value)
{
	if (InventoryManagerRef && InventoryManagerRef->bIsInventoryOpen) return;//dont allow when inventory open
	if (bIsAttacking) return;
	if (UCharacterStatsComp* PlayerStats = FindComponentByClass<UCharacterStatsComp>())
	{
		if (PlayerStats->CurrentStamina > 14)
		{
			PlayerStats->OnStaminaChange(15);
			UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
			if (!animInstance || !AttackMontage) return;
			
			bIsAttacking = true;
			LockRotation(true);
			animInstance->Montage_Play(AttackMontage);
		}
	}
}

void ABaseCharacter::PerformSphereAttack()
{
	HitActorsThisSwing.Empty();
	
	FVector Origin = GetActorLocation() + (GetActorForwardVector() * AttackRange); //geting what is infront of the player
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this); //so i dont hit myself
	
	//setting up the overlap sphere
	TArray<FOverlapResult> HitResults;
	GetWorld()->OverlapMultiByChannel(
	 HitResults,
		Origin,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		CollisionQueryParams
	);
	
	for (const FOverlapResult& Result : HitResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor || HitActorsThisSwing.Contains(HitActor)) continue;
		HitActorsThisSwing.Add(HitActor);
		
		float FinalDamage = AttackDamage;
		if (CharacterStats)
		{
			FinalDamage = CharacterStats->GetTotalDamage(); //grabbing damage off stats component calculated there
		}
		
		UEnemyBaseStatsComp* Enemystats = HitActor->FindComponentByClass<UEnemyBaseStatsComp>();
		if (Enemystats)
		{
			Enemystats->EnemyHealthChange(FinalDamage);
			UE_LOG(LogTemp, Display, TEXT("Damage given: %f"), FinalDamage) 
		}
	}
}

void ABaseCharacter::OnAttackEnd() //call at end
{
	bIsAttacking = false;
	HitActorsThisSwing.Empty();
	LockRotation(false);
}

void ABaseCharacter::OnAttackHitFrame() //call when the attack hits
{
	PerformSphereAttack();
}

void ABaseCharacter::LockRotation(bool bLock)
{
	PlayerMovementComponent->bOrientRotationToMovement = !bLock;
	bUseControllerRotationYaw = bLock;
}
#pragma endregion
