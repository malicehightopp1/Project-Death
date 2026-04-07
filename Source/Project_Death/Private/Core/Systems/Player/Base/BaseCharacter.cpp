// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Default component setups
	PlayerMeshComponent = GetMesh();
	PlayerCapsuleComponent = GetCapsuleComponent();
	
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
	PlayerBloomComponent->SetupAttachment(PlayerCapsuleComponent);
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

