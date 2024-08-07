// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ChairActor.generated.h"

class USwitchComponent;

/**
 * 
 */
UCLASS()
class PANACEA_API AChairActor : public AItem
{
	GENERATED_BODY()

public:
	AChairActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USwitchComponent* SwitchComponent;

	UPROPERTY(EditAnywhere)
	float MinimumDistance;

protected:
	virtual void BeginPlay() override;

public:

	virtual void Interact() override;

private:
	FVector OriginalLocation;
};
