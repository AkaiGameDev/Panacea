#include "GrabbingSystemComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "PanaceaCharacter.h"
#include "Components/InputComponent.h"
#include <EnhancedInputComponent.h>
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabbingSystemComponent::UGrabbingSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabbingSystemComponent::BeginPlay()
{
    Super::BeginPlay();

    FindPhysicsHandle();
    SetupInputComponent();


    AActor* Owner = GetOwner();

    if (Owner)
    {
        APanaceaCharacter* Character = Cast<APanaceaCharacter>(Owner);
        if (Character)
        {
            Crosshair = Character->GetCrosshairWidget();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to cast Owner to APanaceaCharacter"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Owner is null"));
    }
}
// Called every frame
void UGrabbingSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // If the physics handle is attached
    if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
    {
        // Move the object that we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}

void UGrabbingSystemComponent::Grab()
{
    if (/*!PhysicsHandle*/ true) { return; }

    if (PhysicsHandle->GrabbedComponent)
    {
        // If we're currently grabbing something, release it
        UE_LOG(LogTemp, Warning, TEXT("Grab released"));

        // Disable CCD
        PhysicsHandle->GrabbedComponent->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
        PhysicsHandle->GrabbedComponent->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
        PhysicsHandle->GrabbedComponent->SetEnableGravity(true);
        PhysicsHandle->GrabbedComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

        UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GrabbedComponent;
        if (GrabbedComponent)
        {
            GrabbedComponent->SetUseCCD(false);
        }

        PhysicsHandle->ReleaseComponent();
        Crosshair->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {

        // LINE TRACE and see if we reach any actors with physics body collision channel set
        UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
        auto HitResult = GetFirstPhysicsBodyInReach();
        auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
        auto ActorHit = HitResult.GetActor();

        // If we hit something and it has the "Ingredient" tag, then attach a physics handle
        if (ActorHit && ActorHit->ActorHasTag("Ingredient"))
        {
            PhysicsHandle->GrabComponentAtLocation(
                ComponentToGrab,
                NAME_None, // no bones needed
                ComponentToGrab->GetOwner()->GetActorLocation()
            );
            Crosshair->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

void UGrabbingSystemComponent::FindPhysicsHandle()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
    }
}

void UGrabbingSystemComponent::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UEnhancedInputComponent>();
    if (InputComponent)
    {
        InputComponent->BindAction(GrabAction, ETriggerEvent::Triggered, this, &UGrabbingSystemComponent::Grab);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
    }
}

const FHitResult UGrabbingSystemComponent::GetFirstPhysicsBodyInReach()
{
    // Line-trace (AKA ray-cast) out to reach distance
    FHitResult HitResult;
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(
        OUT HitResult,
        GetReachLineStart(),
        GetReachLineEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );

    return HitResult;
}

FVector UGrabbingSystemComponent::GetReachLineStart()
{
    // Get player viewpoint
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    return PlayerViewPointLocation;
}

FVector UGrabbingSystemComponent::GetReachLineEnd()
{
    // Get player viewpoint
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    // Draw a line from the player showing the reach
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

