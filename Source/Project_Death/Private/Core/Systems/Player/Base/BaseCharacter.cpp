// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/BaseCharacter.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/Systems/Interactions/InteractionManager.h"
#include "Core/Systems/Player/Inventory/InventoryManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

	//Creating widget
	PlayerWidget = CreateDefaultSubobject<UUserWidget>(TEXT("PlayerWidget"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerWidget)
	{
		PlayerWidget->AddToViewport();
	}
	InventoryManagerRef = FindComponentByClass<UInventoryManager>();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	}
}

void ABaseCharacter::Interact(const FInputActionValue& Value)
{
	FHitResult* Hit = new FHitResult();
	FVector start = PlayerCameraComponent->GetComponentLocation();
	FVector end = start + (PlayerCameraComponent->GetForwardVector() *1000);

	UE_LOG(LogTemp, Display, TEXT("Interact called"));
	UKismetSystemLibrary::SphereTraceSingle(this, start, end, 5.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		false, TArray<AActor*>(), EDrawDebugTrace::Persistent, *Hit, true);
	
	if (Hit->GetActor() != nullptr)
	{
		if (Hit->GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())) //if the actor that was hit has a interface
		{
			Cast<IInteractInterface>(Hit->GetActor())->InteractPure(this);
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
	InventoryManagerRef->Inventory();
}

void ABaseCharacter::PlayerDodge() //dodge for player
{
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

void ABaseCharacter::PlayerDodgeEnd() //reseting the dodge
{
	bIsDodging = false;
	DodgeDirection = FVector::ZeroVector;
}