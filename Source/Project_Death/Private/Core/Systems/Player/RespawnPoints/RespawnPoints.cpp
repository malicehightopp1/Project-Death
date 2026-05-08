// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/RespawnPoints/RespawnPoints.h"

// Sets default values
ARespawnPoints::ARespawnPoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ARespawnPoints::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

