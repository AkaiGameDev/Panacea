// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "Item.h"
#include "PanaceaCharacter.h"
#include "AlchemyCircleActor.generated.h"

/**
 * 
 */
UCLASS()
class PANACEA_API AAlchemyCircleActor : public AItem
{
	GENERATED_BODY()



	
public:
	AAlchemyCircleActor();

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere,Category="Config")
	FString IngredientNameToPlace;


	UFUNCTION()
	virtual void BeginPlay() override;


	// Called when the sphere overlaps with another actor
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	APanaceaCharacter* PlayerCharacter;
	UPROPERTY()
	UInteractiveComponent* InteractiveComponent;


};
