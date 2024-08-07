// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PanaceaCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MouseDragObjectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComponentMouseRelease, UPrimitiveComponent*, ReleasedComponent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PANACEA_API UMouseDragObjectsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMouseDragObjectsComponent();

	void SwitchGrabMode();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MouseDragObjectsMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabObjectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabModeAction;

	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere)
	float TraceSpehereRadius;

	UPROPERTY(BlueprintAssignable, Category = "Mouse Drag")
	FOnComponentMouseRelease OnComponentMouseRelease;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void InitializeReferences();
	void SetupInputBindings();
	UPrimitiveComponent* PerformTrace(FHitResult& HitResult, float Radius) const;
	void GrabObject();
	void ReleaseObject();


	APanaceaCharacter* Character;
	APlayerController* PlayerController;
	UInputMappingContext* CharacterDefaultMappingContext;
	UPrimitiveComponent* GrabbedComponent;
	bool bIsGrabMode;
	float OriginalDistanceToComponent;
	UUserWidget* Crosshair;
};