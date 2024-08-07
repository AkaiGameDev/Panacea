// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IInteractable.h"

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteractableItem.generated.h"

// Interface containing the methods for any item in the room that can be interacted with. 
UINTERFACE(MinimalAPI)
class UIInteractableItem : public UInteractable
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANACEA_API IIInteractableItem : public IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Broadcast() = 0;
	virtual void CheckInteractable(const FString& itemInteracted) = 0;
	virtual void SetInteractable() = 0;
	virtual void SetNotInteractable() = 0;
};
