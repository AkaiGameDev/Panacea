#include "NoteActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


// Sets default values
ANoteActor::ANoteActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Interactable = true;
}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANoteActor::Interact()
{ 
	UE_LOG(LogTemp, Warning, TEXT("Diary interact called"));
	if (Interactable) {
		UE_LOG(LogTemp, Warning, TEXT("Diary interacted with"));
		Broadcast();



		UE_LOG(LogTemp, Warning, TEXT("Dairy function called"));
		
		    bool bIsGamePaused = UGameplayStatics::IsGamePaused(GetWorld());
		    UE_LOG(LogTemp, Warning, TEXT("Is Game Paused: %s"), bIsGamePaused ? TEXT("True") : TEXT("False"));
		
		    if (bIsGamePaused)
		    {
		        // Unpause the game if it's currently paused
		        if (PauseMenuWidget)
		        {
		            PauseMenuWidget->RemoveFromParent();
		            PauseMenuWidget = nullptr;
		            UE_LOG(LogTemp, Warning, TEXT("Pause Menu Removed"));
		
		            APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		            if (PlayerController)
		            {
		                FInputModeGameOnly InputMode;
		                PlayerController->SetInputMode(InputMode);
		                PlayerController->bShowMouseCursor = false;
		            }
		        }
		
		        UGameplayStatics::SetGamePaused(GetWorld(), false);
		        UE_LOG(LogTemp, Warning, TEXT("Game Unpaused"));
		    }
		    else
		    {
		
		            if (!PauseMenuWidgetClass)
		            {
		                UE_LOG(LogTemp, Error, TEXT("PauseMenuWidgetClass is not set"));
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
		            PauseMenuWidget = CreateWidget<UUserWidget>(PlayerController, PauseMenuWidgetClass);
		            if (!PauseMenuWidget)
		            {
		                UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget could not be created."));
		                return;
		            }
		
		            // Add the widget to the viewport
		            PauseMenuWidget->AddToViewport();

		    		//get text from the note
		            UTextBlock* TextBlock = Cast<UTextBlock>(PauseMenuWidget->GetWidgetFromName(TEXT("Content")));
		            if (TextBlock)
		            {
		                TextBlock->SetText(FText::FromString(NoteText));
		            }


		            // Set the input mode to UI only and show the mouse cursor
		            FInputModeUIOnly InputMode;
		            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		            PlayerController->SetInputMode(InputMode);
		            PlayerController->bShowMouseCursor = true;
		
		            // Pause the game
		            UGameplayStatics::SetGamePaused(this, true);
		            UE_LOG(LogTemp, Warning, TEXT("Game Paused and Pause Menu Displayed"));
		    }

	}
}
