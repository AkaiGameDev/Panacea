// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <InputAction.h>
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PauseMenuComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PANACEA_API UPauseMenuComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPauseMenuComponent();
	/**  Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//UPROPERTY(EditAnywhere, Category = "Pause Menu")
	//TSubclassOf<UUserWidget> PauseMenuClass;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




private:

	// Input Component
	UPROPERTY()
	UEnhancedInputComponent* InputComponent = nullptr;
	void SetupInputComponent();
	void PauseGame();


	UPROPERTY(EditAnywhere)
	UPauseMenuComponent* PauseMenuComponent;

	UPROPERTY(EditAnywhere, Category = "Pause Menu")
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;


};
