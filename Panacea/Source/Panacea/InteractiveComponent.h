// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"

#include "InputActionValue.h"
#include "InteractiveComponent.generated.h"

class APanaceaCharacter;
class AItem;
class UCameraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PANACEA_API UInteractiveComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInteractiveComponent();
    
    void Interact(const FInputActionValue& Value);

    void Grab();

    void Release();

    void ResetActorInFocus(AActor* OtherActor, bool bRemoveActor = true);

    void HideActor(AActor* ActorToHide);

    void SetAndStartMovement(const FVector& TargetVector, const FRotator& TargetRotator, bool bIsRelease);

    UPROPERTY(EditAnywhere)
    FVector GrabbedActorLocationViewport;

    UPROPERTY(EditAnywhere)
    FRotator GrabbedActorRotationViewport;

    UPROPERTY(EditAnywhere)
    float MovementSpeed; // Units per second
    
    UPROPERTY(EditAnywhere)
    float ReleaseDistance;

    UPROPERTY(EditAnywhere)
    bool bIsHolding;
protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    APanaceaCharacter* Owner;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void DrawDebugCapsuleVisualization() const;

    AActor* GetClosestToOwner(const TArray<AActor*>& ActorsToCheck );

    bool IsPathClear(const FVector& StartLocation, const FVector& EndLocation, const FVector& BoxExtent) const;

    void OnMoveItemComplete();

    void SetActorInFocus(AActor* NewActorInFocus);

    void OnTickUpdateItemTransform(float DeltaTime);

    FVector FindPlaceToRelease(FVector& BoxExtent);

    UCapsuleComponent* InteractiveCapsuleCollider;

    TArray<AActor*> InteractableActors;

    AActor* ActorInFocus;

    UPrimitiveComponent* ActorInFocusRootComponent;

    AItem* ActorInFocusInteractable;

    UCameraComponent* CharacterCameraComponent;

    FVector TargetLocationToRelease;

    FRotator TargetRotationToRelease;

    UUserWidget* HintInteractionWidget;

    bool bIsMovingToTarget;

   
};
