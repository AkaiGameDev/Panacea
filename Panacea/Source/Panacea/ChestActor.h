// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ChestActor.generated.h"

class USwitchComponent;

/**
 * 
 */
UCLASS()
class PANACEA_API AChestActor : public AItem
{
	GENERATED_BODY()

public:
	AChestActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* BottomStaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* TopStaticMeshComponent;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	USwitchComponent* SwitchComponent;

	UPROPERTY(EditAnywhere)
	float MinimumDegrees;

protected:
	virtual void BeginPlay() override;

public:

	virtual void Interact() override;

private:
	FVector OriginalAngleVector;
};
