//#pragma once
//#include "Item.h"
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "PhysicsEngine/PhysicsHandleComponent.h"
//#include "DairyComponent.generated.h"
//
//class UInputAction;
//class UUserWidget;
//
//UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
//class PANACEA_API UDairyComponent : public AItem
//{
//	GENERATED_BODY()
//
//public:
//
//	UDairyComponent();
//
//	/** Dairy Input Action */
//	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
//	UInputAction* DairyAction;
//
//protected:
//
//	virtual void BeginPlay() override;
//
//public:
//
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//	virtual void Interact() override;
//	virtual void Broadcast() override;
//
//private:
//	// Physics Handle
//	UPROPERTY()
//	UPhysicsHandleComponent* PhysicsHandle = nullptr;
//
//	// Input Component
//	UPROPERTY()
//	UEnhancedInputComponent* InputComponent = nullptr;
//
//	// Pause Menu Widget Class
//	UPROPERTY(EditAnywhere, Category = "UI")
//	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
//
//	// Instance of the Pause Menu Widget
//	UPROPERTY()
//	UUserWidget* PauseMenuWidget = nullptr;
//
//	// Ray-cast and dairy what's in reach
//	void Dairy();
//
//	void Freeze();
//
//	// Find attached physics handle
//	void FindPhysicsHandle();
//
//	// Setup attached input component
//	void SetupInputComponent();
//
//	// Return hit for first physics body in reach
//	const FHitResult GetFirstPhysicsBodyInReach();
//
//	// Returns current start of reach line
//	FVector GetReachLineStart();
//
//	// Returns current end of reach line
//	FVector GetReachLineEnd();
//
//	// How far ahead of the player can we reach in cm
//	UPROPERTY(EditAnywhere)
//	float Reach = 200.f;
//};
