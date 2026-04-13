// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/MainMenu/MainMenuUI.h"

#include "Components/Button.h"

UMainMenuUI::UMainMenuUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UMainMenuUI::NativeConstruct()
{
	BindWidgets();
}

void UMainMenuUI::BindWidgets()
{
	StartButton->OnClicked.AddDynamic(this, &UMainMenuUI::StartGame);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenuUI::SettingsMenu);
	CreditsButton->OnClicked.AddDynamic(this, &UMainMenuUI::CreditsMenu);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuUI::QuitGame);
	BackButton->OnClicked.AddDynamic(this, &UMainMenuUI::Backbutton);
}

void UMainMenuUI::UnbindWidgets() //removing if necessary
{
	StartButton->OnClicked.RemoveDynamic(this, &UMainMenuUI::StartGame);
	SettingsButton->OnClicked.RemoveDynamic(this, &UMainMenuUI::SettingsMenu);
	CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuUI::CreditsMenu);
	QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuUI::QuitGame);
	BackButton->OnClicked.RemoveDynamic(this, &UMainMenuUI::Backbutton);
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

