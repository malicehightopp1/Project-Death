// Fill out your copyright notice in the Description page of Project Settings.
//CPP is empty for this
#pragma once

#include "InteractionManager.generated.h"
class AMyCharacter; //forward declare to the player character

UINTERFACE(MinimalAPI, Blueprintable) 
class UInteractInterface : public UInterface //included with an interface
{
	GENERATED_BODY()
};

class IInteractInterface //included with an interface
{
	GENERATED_BODY()
	
public:
	UFUNCTION() virtual void InteractPure(ABaseCharacter* player) = 0; //C++ only function
};

