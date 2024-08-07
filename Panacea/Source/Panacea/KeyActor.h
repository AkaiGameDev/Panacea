// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "KeyActor.generated.h"

/**
 * 
 */
UCLASS()
class PANACEA_API AKeyActor : public AItem
{
	GENERATED_BODY()
public:
	virtual void Interact() override;

	UPROPERTY(EditAnywhere)
	FVector ChestLockLocation;

	UPROPERTY(EditAnywhere)
	FRotator ChestLockRotation;
};
