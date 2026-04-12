// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/Base/PlayerCurrency/CurrencyManager.h"

// Sets default values for this component's properties
UCurrencyManager::UCurrencyManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UCurrencyManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentCurrency = 0;
	
	OnCurrencyChanged.Broadcast(CurrentCurrency);
}


// Called every frame
void UCurrencyManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCurrencyManager::OnCurrencyChange(float NewCurrency)
{
	CurrentCurrency += NewCurrency;
	CurrentCurrency = FMath::Clamp(CurrentCurrency, 0.0f, MaxCurrency);
	OnCurrencyChanged.Broadcast(CurrentCurrency);
	UE_LOG(LogTemp, Log, TEXT("Currency: %f"), CurrentCurrency);
}

