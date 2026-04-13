// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUI.generated.h"

class UOverlay;
class UButton;
/**
 * 
 */
UCLASS()
class UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	UMainMenuUI(const FObjectInitializer& ObjectInitializer);
	
private:
	virtual void NativeConstruct();
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Basic Buttons --------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(meta = (BindWidget)) UButton* StartButton;
	
	UPROPERTY(meta = (BindWidget)) UButton* SettingsButton;
	
	UPROPERTY(meta = (BindWidget)) UButton* CreditsButton;
	
	UPROPERTY(meta = (BindWidget)) UButton* QuitButton;
	
	UPROPERTY(meta = (BindWidget)) UButton* BackButton;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Overlays --------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(meta = (BindWidget)) UOverlay* SettingsOverlay;
	UPROPERTY(meta = (BindWidget)) UOverlay* CreditsOverlay;
	UPROPERTY(meta = (BindWidget)) UOverlay* QuitOverlay;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Button Functions -----------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void StartGame();
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void SettingsMenu();
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void CreditsMenu();
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void QuitGame();
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void Backbutton();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Setup ----------------------------------------------------------------------------------------------------------
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void BindWidgets();
	UFUNCTION(BlueprintCallable, Category = "MainMenu") void UnbindWidgets();
};
