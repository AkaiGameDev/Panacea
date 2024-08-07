// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "PhilosophersStoneActor.generated.h"

UCLASS()
class PANACEA_API APhilosophersStoneActor : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhilosophersStoneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void Enable();


private:
	UPROPERTY()
	UStaticMeshComponent* StoneMeshComponent;
	UPROPERTY()
	FVector InitialLocation;

	UPROPERTY(EditAnywhere)
	float Speed = 10.0f;

	UPROPERTY(EditAnywhere)
	float MaxHeight = 200.0f;

	UPROPERTY(EditAnywhere)
	bool bEnabled = false;


	UFUNCTION()
	void MoveStone(float DeltaTime);

};
