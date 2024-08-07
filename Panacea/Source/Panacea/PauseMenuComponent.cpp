#include "PauseMenuComponent.h"
#include "GameFramework/Actor.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h" 

UPauseMenuComponent::UPauseMenuComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPauseMenuComponent::BeginPlay()
{
    Super::BeginPlay();
    SetupInputComponent();
}

void UPauseMenuComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPauseMenuComponent::SetupInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UEnhancedInputComponent>();
    if (InputComponent)
    {
        InputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &UPauseMenuComponent::PauseGame);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
    }
}

void UPauseMenuComponent::PauseGame()
{
    UE_LOG(LogTemp, Warning, TEXT("PAUSE!!!AAAAA"));

    // Check if the pause menu widget class is set
    if (!PauseMenuWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("PauseMenuWidgetClass is not set."));
        return;
    }

    // Get the player controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerController is not found."));
        return;
    }

    // Create the pause menu widget
    UUserWidget* PauseMenuWidget = CreateWidget<UUserWidget>(PlayerController, PauseMenuWidgetClass);
    if (!PauseMenuWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget could not be created."));
        return;
    }

    // Add the widget to the viewport
    PauseMenuWidget->AddToViewport();

    // Set the input mode to UI only and show the mouse cursor
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PlayerController->SetInputMode(InputMode);
    PlayerController->bShowMouseCursor = true;

    // Pause the game
    UGameplayStatics::SetGamePaused(this, true);




}
