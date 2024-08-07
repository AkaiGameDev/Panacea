// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "IInteractable.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "PanaceaCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"  // Ensure this is included
#include "Item.h"

UInteractiveComponent::UInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsHolding = false;
	bIsMovingToTarget = false;

	GrabbedActorLocationViewport = FVector(70.0f, 30.0f, -30.0f);
	GrabbedActorRotationViewport = FRotator::ZeroRotator;

	MovementSpeed = 20.0f; // Units per second
	ReleaseDistance = 220.0f;

	// Create and initialize the capsule component
	InteractiveCapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractiveCapsuleCollider"));
	InteractiveCapsuleCollider->InitCapsuleSize(20.0f, 140.0f); // Set the radius and half-height for the capsule
	InteractiveCapsuleCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	InteractiveCapsuleCollider->SetVisibility(true);

	// Bind the overlap events
	InteractiveCapsuleCollider->OnComponentBeginOverlap.AddDynamic(this, &UInteractiveComponent::OnOverlapBegin);
	InteractiveCapsuleCollider->OnComponentEndOverlap.AddDynamic(this, &UInteractiveComponent::OnOverlapEnd);
}

void UInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("UInteractiveComponent::BeginPlay - Start"));
	// Ensure that the Owner is set
	Owner = Cast<APanaceaCharacter>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is null."));
		return;
	}

	HintInteractionWidget = Owner->GetHintInteractionWidget();

	CharacterCameraComponent = Owner->GetFirstPersonCameraComponent();
	if (!CharacterCameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Camera component is null."));
		return;
	}

	// Cast UCameraComponent to USceneComponent
	USceneComponent* RootComp = Cast<USceneComponent>(CharacterCameraComponent);
	if (!RootComp)
	{
		UE_LOG(LogTemp, Error, TEXT("Casting to USceneComponent failed."));
		return;
	}

	InteractiveCapsuleCollider->AttachToComponent(RootComp, FAttachmentTransformRules::KeepRelativeTransform);
	InteractiveCapsuleCollider->SetRelativeLocation(FVector::ForwardVector * 130.0f);
	InteractiveCapsuleCollider->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	if (HintInteractionWidget)
	{
		HintInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HintInteractionWidget is null in InteractiveComponent::BeginPlay"));
	}
}

void UInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsMovingToTarget || !ActorInFocus || !ActorInFocusRootComponent)
	{
		return;
	}

	OnTickUpdateItemTransform(DeltaTime);
}

void UInteractiveComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Cast<IInteractable>(OtherActor) != nullptr)
	{
		if (bIsMovingToTarget)
			return;

		InteractableActors.AddUnique(OtherActor);

		if (bIsHolding)
			return;

		// Get the closest actor to the owner
		AActor* NewActorInFocus = GetClosestToOwner(InteractableActors);

		if (ActorInFocus != NewActorInFocus)
		{

			// Notify the old actor that it is out of range
			if (ActorInFocusInteractable)
			{
				ActorInFocusInteractable->OnInteractableOutOfRange();
				HintInteractionWidget->SetVisibility(ESlateVisibility::Hidden);

			}

			SetActorInFocus(NewActorInFocus);

			// Notify the new actor that it is in range
			if (ActorInFocusInteractable)
			{
				ActorInFocusInteractable->OnInteractableInRange();

				if (ActorInFocusInteractable->Interactable)
				{
					HintInteractionWidget->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void UInteractiveComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IInteractable* Interactable = Cast<IInteractable>(OtherActor))
	{

		if (bIsMovingToTarget)
			return;

		TArray<AActor*> OverlappingActors;
		GetOwner()->GetOverlappingActors(OverlappingActors);

		if (OverlappingActors.Contains(OtherActor))
			return;

		InteractableActors.Remove(OtherActor);

		if (bIsHolding)
			return;

		// If the actor going out of range was in focus, update the focus
		if (OtherActor == ActorInFocus)
		{
			if (AItem* OldInteractable = Cast<AItem>(ActorInFocus))
			{
				OldInteractable->OnInteractableOutOfRange();
				HintInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		// Get the new closest actor to the owner
		SetActorInFocus(GetClosestToOwner(InteractableActors));

		// Notify the new actor that it is in range
		if (AItem* NewInteractable = Cast<AItem>(ActorInFocus))
		{
			NewInteractable->OnInteractableInRange();

			if (NewInteractable->Interactable)
			{
				HintInteractionWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UInteractiveComponent::Interact(const FInputActionValue& Value)
{
	if (!ActorInFocus)
		return;

	AItem* Item = Cast<AItem>(ActorInFocus);
	if (!Item)
		return;

	if (Item->Interactable)
		Item->Interact();
	else
		return;

	//Check have item changed after Interact function
	AItem* ItemCheck = Cast<AItem>(ActorInFocus);

	if (Item != ItemCheck)
		return;

	if (Item->Grabbable)
	{
		if (!bIsHolding)
		{
			Grab();
		}
		else if (!bIsMovingToTarget)
		{
			Release();
		}
	}
}

void UInteractiveComponent::Grab()
{
	if (Owner && ActorInFocus)
	{
		SetAndStartMovement(FVector(), FRotator(), false);
	}
}

void UInteractiveComponent::Release()
{
	if (Owner && ActorInFocus && bIsHolding)
	{
		if (!CharacterCameraComponent)
			return;

		FVector BoxExtent;
		TargetLocationToRelease = FindPlaceToRelease(BoxExtent);

		if (TargetLocationToRelease.IsNearlyZero())
			return;

		FVector CameraLocation = CharacterCameraComponent->GetComponentLocation();

		if (IsPathClear(CameraLocation, TargetLocationToRelease, BoxExtent))
		{
			FRotator CurrentRotation = ActorInFocusRootComponent->GetComponentRotation();

			SetAndStartMovement(TargetLocationToRelease, FRotator(0.0f, CurrentRotation.Yaw, 0.0f), true);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Path is not clear, cannot release actor."));
		}

	}
}

bool UInteractiveComponent::IsPathClear(const FVector& StartLocation, const FVector& EndLocation, const FVector& BoxExtent) const
{
	FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);
	FHitResult HitResult;

	// Perform the box trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ActorInFocus);
	QueryParams.AddIgnoredActor(Owner);

	// Perform the box trace
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		Box,
		QueryParams
	);

	//DrawDebugBox(GetWorld(), StartLocation, BoxExtent, FColor::Red, false, 1.0f);
	//DrawDebugBox(GetWorld(), EndLocation, BoxExtent, FColor::Green, false, 1.0f);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 1.0f, 0, 1.0f);
	//if (HitResult.bBlockingHit)
	//	DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 4.0f);


	return !bHit; // Return true if the path is clear
}

void UInteractiveComponent::OnMoveItemComplete()
{
	if (!Owner || !ActorInFocus)
		return;

	bIsHolding = !bIsHolding;

	if (bIsHolding)
	{
		if (CharacterCameraComponent && ActorInFocusRootComponent && ActorInFocusInteractable)
		{
			ActorInFocusRootComponent->AttachToComponent(CharacterCameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			ActorInFocusInteractable->OnInteractableOutOfRange();
		}
	}
	else
	{
		TArray<AActor*> OverlappingActors;
		InteractiveCapsuleCollider->GetOverlappingActors(OverlappingActors);

		if (!OverlappingActors.Contains(ActorInFocus))
		{
			ResetActorInFocus(ActorInFocus);
		}

		if (ActorInFocusInteractable)
		{
			ActorInFocusInteractable->OnInteractableOutOfRange();
		}

		// Get the new closest actor to the owner
		SetActorInFocus(GetClosestToOwner(InteractableActors));

		// Notify the new actor that it is in range
		if (ActorInFocusInteractable)
		{
			ActorInFocusInteractable->OnInteractableInRange();
		}
	}

}

AActor* UInteractiveComponent::GetClosestToOwner(const TArray<AActor*>& ActorsToCheck)
{
	AActor* ClosestActor = nullptr;
	float ClosestDistance = FLT_MAX;

	if (!CharacterCameraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Camera component is null."));
		return nullptr;
	}

	// Iterate through the list of actors to find the closest one
	for (AActor* InteractableActor : ActorsToCheck)
	{
		AItem* Item = Cast<AItem>(InteractableActor);

		if (Item && !Item->Interactable)
		{
			continue;
		}

		if (InteractableActor)
		{
			UStaticMeshComponent* StaticMesh = InteractableActor->GetComponentByClass<UStaticMeshComponent>();

			FVector Start = CharacterCameraComponent->GetComponentLocation();
			FVector End = StaticMesh ? End = StaticMesh->GetComponentLocation() : End = InteractableActor->GetActorLocation();

			FHitResult HitResult;

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Owner);

			// Perform the line trace
			bool bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				Start,
				End,
				ECC_Visibility,
				QueryParams
			);

			if (HitResult.GetActor() == InteractableActor)
			{
				float Distance = FVector::Dist(Start, End);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestActor = InteractableActor;
				}
			}

			// Debug drawing
			//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
		}
	}

	return ClosestActor;
}

void UInteractiveComponent::SetActorInFocus(AActor* NewActorInFocus)
{
	ActorInFocus = NewActorInFocus;
	if (ActorInFocus)
	{
		ActorInFocusRootComponent = Cast<UPrimitiveComponent>(ActorInFocus->GetRootComponent());
		ActorInFocusInteractable = Cast<AItem>(ActorInFocus);
	}
	else
	{
		ActorInFocusInteractable = nullptr;
		ActorInFocusRootComponent = nullptr;
	}
}

void UInteractiveComponent::ResetActorInFocus(AActor* OtherActor, bool bRemoveActor /*= true*/)
{
	if (bRemoveActor)
		InteractableActors.Remove(OtherActor);

	// If the actor going out of range was in focus, update the focus
	if (OtherActor == ActorInFocus)
	{
		if (!ActorInFocusInteractable)
			return;

		HideActor(ActorInFocus);

		bIsHolding = false;
	}

	if (bIsHolding)
		return;

	// Get the new closest actor to the owner
	SetActorInFocus(GetClosestToOwner(InteractableActors));

	// Notify the new actor that it is in range
	if (IInteractable* InteractableToCast = Cast<IInteractable>(ActorInFocus))
	{
		InteractableToCast->OnInteractableInRange();

		AItem* Item = Cast<AItem>(ActorInFocus);

		if (Item && Item->Interactable)
		{
			HintInteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UInteractiveComponent::HideActor(AActor* ActorToHide)
{
	AItem* Ingredient = Cast<AItem>(ActorToHide);
	if (!Ingredient)
		return;

	Ingredient->OnInteractableOutOfRange();
	HintInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UInteractiveComponent::SetAndStartMovement(const FVector& TargetVector, const FRotator& TargetRotator, bool bIsRelease)
{
	if (ActorInFocusRootComponent)
	{
		if (bIsRelease)
		{
			TargetLocationToRelease = TargetVector;
			TargetRotationToRelease = TargetRotator;

			ActorInFocusRootComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		}
		else
		{
			if (!CharacterCameraComponent)
			{
				return;
			}

			ActorInFocusRootComponent->AttachToComponent(CharacterCameraComponent, FAttachmentTransformRules::KeepWorldTransform);
		}

		bIsMovingToTarget = true;
	}
}

void UInteractiveComponent::OnTickUpdateItemTransform(float DeltaTime)
{

	FVector CurrentLocation = bIsHolding ? ActorInFocusRootComponent->GetComponentLocation() : ActorInFocusRootComponent->GetRelativeLocation();
	FRotator CurrentRotation = bIsHolding ? ActorInFocusRootComponent->GetComponentRotation() : ActorInFocusRootComponent->GetRelativeRotation();

	FVector DesiredLocation = bIsHolding ? TargetLocationToRelease : GrabbedActorLocationViewport;
	FRotator DesiredRotation = bIsHolding ? TargetRotationToRelease : GrabbedActorRotationViewport;

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, DesiredLocation, DeltaTime, MovementSpeed);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, MovementSpeed);

	if (bIsHolding)
	{
		ActorInFocusRootComponent->SetWorldLocation(NewLocation);
		ActorInFocusRootComponent->SetWorldRotation(NewRotation);
	}
	else
	{
		ActorInFocusRootComponent->SetRelativeLocation(NewLocation);
		ActorInFocusRootComponent->SetRelativeRotation(NewRotation);
	}

	bool bLocationReached = FVector::Dist(NewLocation, DesiredLocation) < 1.0f;
	bool bRotationReached = FQuat::Slerp(CurrentRotation.Quaternion(), DesiredRotation.Quaternion(), DeltaTime * MovementSpeed).Equals(DesiredRotation.Quaternion(), 1.0f);

	if (bLocationReached && bRotationReached)
	{
		bIsMovingToTarget = false;
		OnMoveItemComplete();
	}
}

FVector UInteractiveComponent::FindPlaceToRelease(FVector& BoxExtent)
{
	if (!CharacterCameraComponent)
		return FVector();

	FVector CameraLocation = CharacterCameraComponent->GetComponentLocation();
	FVector CameraForward = CharacterCameraComponent->GetForwardVector();

	BoxExtent = FVector(10.0f, 10.0f, 10.0f); // Define default the box extent for collision checking

	UStaticMeshComponent* MeshComponent = ActorInFocus->FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent)
	{
		FVector Origin, BoxBounds;
		MeshComponent->GetLocalBounds(Origin, BoxBounds);
		FVector ActorScale = MeshComponent->GetComponentScale();
		BoxExtent = BoxBounds * ActorScale; // Half size to get the box extent
	}

	FVector TraceEnd = CameraLocation + (CameraForward * ReleaseDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ActorInFocus);
	QueryParams.AddIgnoredActor(Owner);

	FHitResult HitResult;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	//// Debug drawing
	/*DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Green, false, 1.0f, 0, 1.0f);
	if (bHit)
	{
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Red, false, 1.0f);
	}*/

	if (!bHit)
	{
		UE_LOG(LogTemp, Log, TEXT("Place to put object is not found."));
		return FVector();
	}

	return HitResult.ImpactPoint + FVector::UpVector * (BoxExtent.Z + 0.05f);
}

void UInteractiveComponent::DrawDebugCapsuleVisualization() const
{
	if (AActor* OwnerActor = GetOwner())
	{
		FVector Location = InteractiveCapsuleCollider->GetComponentLocation();
		float HalfHeight = InteractiveCapsuleCollider->GetScaledCapsuleHalfHeight();
		float Radius = InteractiveCapsuleCollider->GetScaledCapsuleRadius();
		FQuat Rotation = InteractiveCapsuleCollider->GetComponentQuat();
		DrawDebugCapsule(GetWorld(), Location, HalfHeight, Radius, Rotation, FColor::Red, false, -1, 0, 0.3);
	}
}