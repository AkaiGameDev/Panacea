#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Blueprint/UserWidget.h"
#include "GrabbingSystemComponent.generated.h"

class UInputAction;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PANACEA_API UGrabbingSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrabbingSystemComponent();

	/** Grab Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Physics Handle
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Input Component
	UPROPERTY()
	UEnhancedInputComponent* InputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();

	// Called when grab is released
	//void Release();

	// Find attached physics handle
	void FindPhysicsHandle();

	// Setup attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current start of reach line
	FVector GetReachLineStart();

	// Returns current end of reach line
	FVector GetReachLineEnd();


	// How far ahead of the player can we reach in cm
	UPROPERTY(EditAnywhere)
	float Reach = 400.f;

	// Crosshair
	UUserWidget* Crosshair;
};
