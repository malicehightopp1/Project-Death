// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

}

void ABaseCharacter::PlayerDodge() //dodge for player
{
	if (bIsDodging) return; //dont allow player to dodge if already dodging

	bIsDodging = true;

	// Get the direction based on player input
	// Uses last movement input so dodge goes the direction you're moving
	FVector DodgeDirection = GetLastMovementInputVector();
	
	if (DodgeDirection.IsNearlyZero()) //if there no input dodgfe backwards
	{
		DodgeDirection = -GetActorForwardVector();
	}

	DodgeDirection.Z = 0.f; // keep it horizontal
	DodgeDirection.Normalize();

	LaunchCharacter(DodgeDirection * DodgeForce, true, true); //pushing the player

	// Reset after dodge duration
	GetWorldTimerManager().SetTimer(
		DodgeTimerHandle,
		this,
		&ABaseCharacter::PlayerDodgeEnd,
		DodgeDuration,
		false
	);
}

void ABaseCharacter::PlayerDodgeEnd() //reseting the dodge
{
	bIsDodging = false;
}