// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "IInteractable.h"
#include "PanaceaGameMode.h"
#include "IInteractableItem.h"
#include "Item.generated.h"

UCLASS(ABSTRACT)
class PANACEA_API AItem : public AActor, public IIInteractableItem
{
	GENERATED_BODY()


public:

	AItem() {

	}

	UPROPERTY(EditAnywhere)
	bool Interactable;

	UPROPERTY(EditAnywhere)
	FString InteractableTrigger;

	//Flag that controls whether the player can pick up the item
	UPROPERTY(EditAnywhere)
	bool Grabbable;

	virtual void BeginPlay() override {

		Super::BeginPlay();

		APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (!GameMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
			return;
		}

		// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;
		GameMode->OnItemInteractedDelegate.AddDynamic(this, &AItem::CheckInteractable);
		//GameMode->OnItemFirstInteractedDelegate.AddDynamic(this, &AItem::FirstInteraction);
	}

	virtual void Broadcast() override {
		APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (!GameMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("%s broadcasted"), *GetActorNameOrLabel());
		GameMode->OnItemInteractedDelegate.Broadcast(GetActorNameOrLabel());
	}

	virtual void OnInteractableInRange() override {

		if (!Interactable) {
			return;
		}

		TArray<UStaticMeshComponent*> MeshComponents;
		GetComponents<UStaticMeshComponent>(MeshComponents);

		for (UStaticMeshComponent* MeshComponent : MeshComponents)
		{
			if (MeshComponent)
				MeshComponent->SetRenderCustomDepth(true);
		}
	}
	virtual void OnInteractableOutOfRange() override {

		TArray<UStaticMeshComponent*> MeshComponents;
		GetComponents<UStaticMeshComponent>(MeshComponents);

		for (UStaticMeshComponent* MeshComponent : MeshComponents)
		{
			if (MeshComponent)
				MeshComponent->SetRenderCustomDepth(false);
		}
	}

	virtual void Interact() override {
		Broadcast();
	}

	UFUNCTION()
	virtual void FirstInteraction()
	{
		APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (!GameMode)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
			return;
		}

		FString ItemID = "FT_" + GetActorNameOrLabel();

		bool bIsFirst = !GameMode->GetItemNames().Contains(ItemID);
		if (bIsFirst)
			GameMode->OnItemInteractedDelegate.Broadcast(ItemID);
	}

	UFUNCTION()
	virtual void CheckInteractable(const FString& itemInteracted) override {
		UE_LOG(LogTemp, Warning, TEXT("Check interactable called by %s"), *itemInteracted);
		if (itemInteracted == InteractableTrigger) {
			SetInteractable();
		}
	}
	virtual void SetInteractable() override {
		UE_LOG(LogTemp, Warning, TEXT("Ingredient set interactable!!"));
		Interactable = true;
	}
	virtual void SetNotInteractable() override {
		Interactable = false;
	}

};
