// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Systems/Player/PlayerUI/PlayerWidget.h"

#include "Components/Image.h"
#include "Components/VerticalBoxSlot.h"

UPlayerWidget::UPlayerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //Constructor 
{

}

void UPlayerWidget::NativeConstruct() //Begin play
{
	Super::NativeConstruct();
	if (PlayerCursorImage) // setting the padding for 
	{
		if (UVerticalBoxSlot* ImageSlot = Cast<UVerticalBoxSlot>(PlayerCursorImage->Slot))
		{
			FMargin NewPadding(10, 10, 10, 10);
			UE_LOG(LogTemp, Warning, TEXT("Set new padding"));
			ImageSlot->SetPadding(NewPadding);
		}
	}
}
