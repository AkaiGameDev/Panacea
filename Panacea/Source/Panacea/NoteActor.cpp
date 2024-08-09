#include "NoteActor.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


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

	PlayerCharacter = Cast<APanaceaCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not found in note actor."));
		return;
	}
	CharacterDefaultMappingContext = PlayerCharacter->GetDefaultMappingContext();
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
	if (!NoteWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget is not initialized"));
		return;
	}

	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not initialized"));
		return;
	}

	NoteWidget->RemoveFromParent();
	NoteWidget = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Pause Menu Removed"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}


	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(CharacterDefaultMappingContext, 0);
	}

	Broadcast();
	bOpened = false;
}

FString ANoteActor::ReplaceLineBreakPlaceholder(const FString& OriginalText)
{
	// Define the placeholder and the actual line break character
	const FString Placeholder = TEXT("<<br>>");
	const FString LineBreak = TEXT("\n");

	// Replace all occurrences of the placeholder with the line break
	FString ProcessedText = OriginalText.Replace(*Placeholder, *LineBreak);

	return ProcessedText;
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
	NoteWidget = CreateWidget<UNoteWidget>(PlayerController, PauseMenuWidgetClass);
	if (!NoteWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseMenuWidget could not be created."));
		return;
	}

	NoteWidget->SetNoteActor(this);
	// Add the widget to the viewport
	NoteWidget->AddToViewport();

	//get text from the note
	UTextBlock* TextBlock = Cast<UTextBlock>(NoteWidget->GetWidgetFromName(TEXT("Content")));
	if (TextBlock)
	{
		FString TextWithLineBreaks = ReplaceLineBreakPlaceholder(NoteText);
		TextBlock->SetText(FText::FromString(TextWithLineBreaks));
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
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(CharacterDefaultMappingContext);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is not found in note actor."));
	}

	bOpened = true;
}
