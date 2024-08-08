#include "NoteActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

TArray<ANoteActor*> ANoteActor::NoteActors;

// Sets default values
ANoteActor::ANoteActor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Interactable = true;

	NoteActors.Add(this);
}


// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not found in note actor."));
	}
}

void ANoteActor::Interact()
{
	if (Interactable)
	{
		if (bOpened)
		{
			CloseNote();
		}
		else
		{
			OpenNote();
		}
	}
}

void ANoteActor::CloseNote()
{
	if (!PauseMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget is not initialized"));
		return;
	}

	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not initialized"));
		return;
	}

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

	// Enable player movement
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bOpened = false;
	Broadcast();
}

void ANoteActor::OpenNote()
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
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);


	//add input to input mode

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;

	// Disable player movement
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not found in note actor."));
	}

	bOpened = true;
}

ANoteActor* ANoteActor::FindOpenedNoteActor()
{
	ANoteActor* OpenedNoteActor = nullptr;

	for (auto note : NoteActors)
	{
		if (note->IsOpened())
		{
			OpenedNoteActor = note;
			break;
		}
	}

	if (OpenedNoteActor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("OpenedNoteActor is not found."));
	}
	return OpenedNoteActor;
}


bool ANoteActor::IsOpened()
{
	return bOpened;
}
