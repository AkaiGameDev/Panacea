// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CauldronCollisionSphereActor.generated.h"


UCLASS()
class PANACEA_API ACauldronCollisionSphereActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACauldronCollisionSphereActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Collision component
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

private:
	void OnIngredientAdded(AActor* Ingredient);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the sphere overlaps with another actor
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);




};
