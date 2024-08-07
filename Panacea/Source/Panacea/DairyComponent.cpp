//#include "DairyComponent.h"
//#include "GameFramework/Actor.h"
//#include "Engine/World.h"
//#include "DrawDebugHelpers.h"
//#include "GameFramework/PlayerController.h"
//#include "Components/InputComponent.h"
//#include <EnhancedInputComponent.h>
//#include "Blueprint/UserWidget.h"
//#include "Kismet/GameplayStatics.h"
//
//// Sets default values for this component's properties
//UDairyComponent::UDairyComponent()
//{
//    PrimaryComponentTick.bCanEverTick = true;
//    //Interactable = true;
//}
//
//// Called when the game starts
//void UDairyComponent::BeginPlay()
//{
//    Super::BeginPlay();
//
//    FindPhysicsHandle();
//    SetupInputComponent();
//}
//
//// Called every frame
//void UDairyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//    // If the physics handle is attached
//    if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
//    {
//        // Move the object that we're holding
//        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
//    }
//}
//
//void UDairyComponent::Interact()
//{
//    UE_LOG(LogTemp, Warning, TEXT("Dairy interacted with"));
//    Broadcast();
//}
//
//void UDairyComponent::Broadcast()
//{
//    FString name = GetOwner()->GetName();
//    OnItemInteractedDelegate.Broadcast(name);
//}
//
////void UDairyComponent::Broadcast()
////{
////    OnDiaryReadDelegate.Broadcast();
////}
//
//void UDairyComponent::Dairy()
//{
//    UE_LOG(LogTemp, Warning, TEXT("Dairy function called"));
//
//    bool bIsGamePaused = UGameplayStatics::IsGamePaused(GetWorld());
//    UE_LOG(LogTemp, Warning, TEXT("Is Game Paused: %s"), bIsGamePaused ? TEXT("True") : TEXT("False"));
//
//    if (bIsGamePaused)
//    {
//        // Unpause the game if it's currently paused
//        if (PauseMenuWidget)
//        {
//            PauseMenuWidget->RemoveFromViewport();
//            PauseMenuWidget = nullptr;
//            UE_LOG(LogTemp, Warning, TEXT("Pause Menu Removed"));
//
//            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//            if (PlayerController)
//            {
//                FInputModeGameOnly InputMode;
//                PlayerController->SetInputMode(InputMode);
//                PlayerController->bShowMouseCursor = false;
//            }
//        }
//
//        UGameplayStatics::SetGamePaused(GetWorld(), false);
//        UE_LOG(LogTemp, Warning, TEXT("Game Unpaused"));
//    }
//    else
//    {
//        auto HitResult = GetFirstPhysicsBodyInReach();
//        auto ActorHit = HitResult.GetActor();
//
//        // Only proceed if the actor has the "Ingredient" tag
//        if (ActorHit && ActorHit->ActorHasTag("Dairy"))
//        {
//            UE_LOG(LogTemp, Warning, TEXT("Ingredient Detected: %s"), *ActorHit->GetName());
//
//            if (!PauseMenuWidgetClass)
//            {
//                UE_LOG(LogTemp, Error, TEXT("PauseMenuWidgetClass is not set"));
//                return;
//            }
//
//            // Get the player controller
//            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//            if (!PlayerController)
//            {
//                UE_LOG(LogTemp, Error, TEXT("PlayerController is not found."));
//                return;
//            }
//
//            // Create the pause menu widget
//            PauseMenuWidget = CreateWidget<UUserWidget>(PlayerController, PauseMenuWidgetClass);
//            if (!PauseMenuWidget)
//            {
//                UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget could not be created."));
//                return;
//            }
//
//            // Add the widget to the viewport
//            PauseMenuWidget->AddToViewport();
//
//            // Set the input mode to UI only and show the mouse cursor
//            FInputModeUIOnly InputMode;
//            InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
//            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
//            PlayerController->SetInputMode(InputMode);
//            PlayerController->bShowMouseCursor = true;
//
//            // Pause the game
//            UGameplayStatics::SetGamePaused(this, true);
//            UE_LOG(LogTemp, Warning, TEXT("Game Paused and Pause Menu Displayed"));
//        }
//        else
//        {
//            UE_LOG(LogTemp, Warning, TEXT("No valid actor with 'Ingredient' tag in reach"));
//        }
//    }
//}
//
//void UDairyComponent::FindPhysicsHandle()
//{
//    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
//    if (PhysicsHandle == nullptr)
//    {
//        UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
//    }
//}
//
//void UDairyComponent::SetupInputComponent()
//{
//    InputComponent = GetOwner()->FindComponentByClass<UEnhancedInputComponent>();
//    if (InputComponent)
//    {
//        InputComponent->BindAction(DairyAction, ETriggerEvent::Triggered, this, &UDairyComponent::Dairy);
//        UE_LOG(LogTemp, Warning, TEXT("Input component and action bound"));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
//    }
//}
//
//const FHitResult UDairyComponent::GetFirstPhysicsBodyInReach()
//{
//    // Line-trace (AKA ray-cast) out to reach distance
//    FHitResult HitResult;
//    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
//    GetWorld()->LineTraceSingleByObjectType(
//        OUT HitResult,
//        GetReachLineStart(),
//        GetReachLineEnd(),
//        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
//        TraceParams
//    );
//
//    return HitResult;
//}
//
//FVector UDairyComponent::GetReachLineStart()
//{
//    // Get player viewpoint
//    FVector PlayerViewPointLocation;
//    FRotator PlayerViewPointRotation;
//    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
//        OUT PlayerViewPointLocation,
//        OUT PlayerViewPointRotation
//    );
//
//    return PlayerViewPointLocation;
//}
//
//FVector UDairyComponent::GetReachLineEnd()
//{
//    // Get player viewpoint
//    FVector PlayerViewPointLocation;
//    FRotator PlayerViewPointRotation;
//    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
//        OUT PlayerViewPointLocation,
//        OUT PlayerViewPointRotation
//    );
//
//    // Draw a line from the player showing the reach
//    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
//}
