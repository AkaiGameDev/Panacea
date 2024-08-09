// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DiaryActor.h"

void ADiaryActor::OpenNote()
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
	UTextBlock* TextBlock1 = Cast<UTextBlock>(NoteWidget->GetWidgetFromName(TEXT("Page1Text")));
	if (TextBlock1)
	{
		FString TextWithLineBreaks = ReplaceLineBreakPlaceholder(PageOneText);
		TextBlock1->SetText(FText::FromString(TextWithLineBreaks));
	}

	UTextBlock* TextBlock2 = Cast<UTextBlock>(NoteWidget->GetWidgetFromName(TEXT("Page2Text")));
	if (TextBlock2)
	{
		FString TextWithLineBreaks = ReplaceLineBreakPlaceholder(PageTwoText);
		TextBlock2->SetText(FText::FromString(TextWithLineBreaks));
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
