// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseDragObjectsComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#include "DrawDebugHelpers.h" // For debugging purposes
#include "CollisionQueryParams.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/KismetSystemLibrary.h"

UMouseDragObjectsComponent::UMouseDragObjectsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsGrabMode = false;
	TraceSpehereRadius = 2.0f;
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("MousePhysicsHandle"));
}

void UMouseDragObjectsComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializeReferences();
	SetupInputBindings();
}

void UMouseDragObjectsComponent::InitializeReferences()
{
	Character = Cast<APanaceaCharacter>(GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is not of class APanaceaCharacter"));
		return;
	}

	PlayerController = Cast<APlayerController>(Character->GetController());
	if (PlayerController)
	{
		CharacterDefaultMappingContext = Character->GetDefaultMappingContext();
		Crosshair = Character->GetCrosshairWidget();
	}
}

void UMouseDragObjectsComponent::SetupInputBindings()
{
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MouseDragObjectsMappingContext, 1);
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				EnhancedInputComponent->BindAction(GrabModeAction, ETriggerEvent::Triggered, this, &UMouseDragObjectsComponent::SwitchGrabMode);
				EnhancedInputComponent->BindAction(GrabObjectAction, ETriggerEvent::Started, this, &UMouseDragObjectsComponent::GrabObject);
				EnhancedInputComponent->BindAction(GrabObjectAction, ETriggerEvent::Completed, this, &UMouseDragObjectsComponent::ReleaseObject);
			}
		}
	}
}

void UMouseDragObjectsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsGrabMode || !GrabbedComponent || !GrabbedComponent->IsSimulatingPhysics())
	{
		return;
	}

	FVector WorldPosition;
	FVector WorldDirection;
	PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	if (!WorldPosition.IsNearlyZero())
	{
		FVector TargetLocation = WorldPosition + WorldDirection * OriginalDistanceToComponent;
		PhysicsHandle->SetTargetLocation(TargetLocation);
	}
}

void UMouseDragObjectsComponent::SwitchGrabMode()
{
	ReleaseObject();

	bIsGrabMode = !bIsGrabMode;
	if (bIsGrabMode)
	{
		Crosshair->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(CharacterDefaultMappingContext);
		}
	}
	else
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
		Crosshair->SetVisibility(ESlateVisibility::Visible);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterDefaultMappingContext, 0);
		}
	}
}

UPrimitiveComponent* UMouseDragObjectsComponent::PerformTrace(FHitResult& HitResult, float Radius) const
{
	FVector WorldPosition;
	FVector WorldDirection;
	PlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FVector TraceEnd = WorldDirection * 10000.0f + WorldPosition;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(World, WorldPosition, TraceEnd, Radius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true);

	//DrawDebugSphere(World, bHit ? HitResult.ImpactPoint : TraceEnd, Radius, 12, FColor::Red, false, 3.0f);

	return bHit ? HitResult.Component.Get() : nullptr;
}

void UMouseDragObjectsComponent::GrabObject()
{
	if (!bIsGrabMode) return;

	FHitResult HitResult;
	GrabbedComponent = PerformTrace(HitResult, TraceSpehereRadius);

	if (!GrabbedComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("No component found to grab"));
		return;
	}

	OriginalDistanceToComponent = HitResult.Distance;

	if (GrabbedComponent->Mobility != EComponentMobility::Movable)
	{
		UE_LOG(LogTemp, Log, TEXT("Component is not movable"));
		return;
	}

	if (!GrabbedComponent->IsSimulatingPhysics())
	{
		UE_LOG(LogTemp, Log, TEXT("Component is not simulating physics"));
		return;
	}

	PhysicsHandle->GrabComponentAtLocation(GrabbedComponent, NAME_None, HitResult.Location);
}

void UMouseDragObjectsComponent::ReleaseObject()
{
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
		GrabbedComponent = nullptr;

		// Notify via delegate
		if (OnComponentMouseRelease.IsBound())
		{
			OnComponentMouseRelease.Broadcast(GrabbedComponent);
		}
	}
}