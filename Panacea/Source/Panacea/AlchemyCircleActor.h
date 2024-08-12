// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "Item.h"
#include "PanaceaCharacter.h"
#include "AlchemyCircleActor.generated.h"

class UParticleSystemComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere,Category="Config")
	FString IngredientNameToPlace;

	// The particle system component
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	UParticleSystemComponent* ParticleSystemComponent;

	UFUNCTION()
	virtual void BeginPlay() override;

	// Called when the sphere overlaps with another actor
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	APanaceaCharacter* PlayerCharacter;
	UPROPERTY()
	UInteractiveComponent* InteractiveComponent;

	AActor* FindActorByName(UWorld* World, FName ActorName);

	AActor* PossibleActorToPlace;

	bool bIsGlowing;
};
