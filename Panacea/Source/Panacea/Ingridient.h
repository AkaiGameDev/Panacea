// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"

class USwitchComponent;

#include "Ingridient.generated.h"



UCLASS()
class PANACEA_API AIngridient : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngridient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interact override
	virtual void Interact() override;
	virtual void OnInteractableInRange() override;
	virtual void OnInteractableOutOfRange() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USwitchComponent* SwitchComponent;

private:

	void SetMaterial(UMaterialInterface* NewMaterial);

};
