// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "SwitchComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PANACEA_API USwitchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitchComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	AActor* OriginalViewTarget;

public:	

	void SwitchCamera();
	void SetupAttachment(TObjectPtr<USceneComponent> Object);



	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* ObjectCamera;
};
