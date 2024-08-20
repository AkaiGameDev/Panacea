// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "Item.h"
#include "PanaceaCharacter.h"
#include "AlchemyCircleActor.generated.h"

class UNiagaraSystem;
class UAudioComponent;
class UMetaSoundSource;

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

	// The Niagara system asset
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Audio")
	UAudioComponent* MetaSoundAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Audio")
	UMetaSoundSource* MetaSoundSource;

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

	bool bIsGlowing;
};
