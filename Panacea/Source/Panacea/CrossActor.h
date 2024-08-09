// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "CrossActor.generated.h"

class USwitchComponent;
class UInteractiveComponent;

/**
 * 
 */
UCLASS()
class PANACEA_API ACrossActor : public AItem
{
	GENERATED_BODY()
	
public:
	ACrossActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USwitchComponent* SwitchComponent;

	UPROPERTY(EditAnywhere)
	float MinimumDegrees;

protected:
 	virtual void BeginPlay() override;

public:

	virtual void Interact() override;

	virtual void Tick(float DeltaTime) override;
private:
	FVector OriginalAngleVector;

	UInteractiveComponent* InteractiveComponent;
};
