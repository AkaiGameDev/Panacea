// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "MetaSoundSource.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootstepComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANACEA_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootstepComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	void CheckVelocity();

	UPROPERTY(EditAnywhere)
	float VelocityThreshold;

	UPROPERTY(EditAnywhere)
	float TimeGap;

	UPROPERTY(EditAnywhere)
	USoundBase* FootstepSound;

private:
	ACharacter* OwnerCharacter;

	FTimerHandle TimerHandle;

};
