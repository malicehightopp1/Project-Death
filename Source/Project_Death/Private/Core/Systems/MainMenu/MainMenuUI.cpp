// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/MainMenu/MainMenuUI.h"

#include "Components/Button.h"

UMainMenuUI::UMainMenuUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainMenuUI::NativeOnInitialized() //this will run right after being added to viewport
{
	Super::NativeOnInitialized();
	BindWidgets();
}

void UMainMenuUI::BindWidgets()
{
	if (IsValid(StartButton))
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::StartGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StartButton is NULL"));
	}
	if (IsValid(SettingsButton))
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuUI::SettingsMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SettingsButton is NULL"));
	}
	if (IsValid(CreditsButton))
	{
		CreditsButton->OnClicked.AddDynamic(this, &UMainMenuUI::CreditsMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CreditsButton is NULL"));
	}
	if (IsValid(QuitButton))
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuUI::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("QuitButton is NULL"));
	}
	if (IsValid(BackButton))
	{
		BackButton->OnClicked.AddDynamic(this, &UMainMenuUI::Backbutton);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BackButton is NULL"));
	}
}

void UMainMenuUI::StartGame()
{
	UE_LOG(LogTemp, Log, TEXT("Starting Game"));
}

void UMainMenuUI::SettingsMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Settings Menu"));
}

void UMainMenuUI::CreditsMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Credits Menu"));
}

void UMainMenuUI::QuitGame()
{
	UE_LOG(LogTemp, Log, TEXT("Quit Game"));
}

void UMainMenuUI::Backbutton()
{
	UE_LOG(LogTemp, Log, TEXT("Backbutton"));
}

