// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UOverlay;
class UCanvasPanel;

UCLASS()
class UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	UPlayerWidget(const FObjectInitializer& ObjectInitializer); //This is what a userwidget constructor looks like
	virtual void NativeConstruct() override; //treated as begin play starts when added to viewport
protected:
	//Base for UI *HOLDERS*
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UCanvasPanel* CanvasPanel;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UOverlay* BaseHolderOverlay;
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UOverlay* PlayerCursorOverlay;
	
	//Player cursor
	UPROPERTY(EditAnywhere, meta = (BindWidget)) class UImage* PlayerCursorImage;
};
