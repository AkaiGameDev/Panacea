// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "PanaceaGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DialogueRowBase.h"

// Sets default values
ADialogueManagerActor::ADialogueManagerActor()
{
	TimeShow = 15.0f;
}

// Called when the game starts or when spawned
void ADialogueManagerActor::BeginPlay()
{
	Super::BeginPlay();

	APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
		return;
	}

	GameMode->OnItemInteractedDelegate.AddDynamic(this, &ADialogueManagerActor::ShowDialogue);


	if (DialogueWidgetClass)
	{
		DialogueWidget = CreateWidget<UUserWidget>(GetWorld(), DialogueWidgetClass);

		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			DialogueWidget->SetVisibility(ESlateVisibility::Hidden);

			//get text block from widget
			DialogueTextBlock = Cast<UTextBlock>(DialogueWidget->GetWidgetFromName(TEXT("Dialogue")));

			if (!DialogueTextBlock)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to get Dialogue text block"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Dialogue widget"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DialogueWidgetClass is null"));
	}

	Character = Cast<APanaceaCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}


void ADialogueManagerActor::ShowDialogue(const FString& ItemName)
{
    static const FString ContextString(TEXT("GENERAL"));

    if (!MyDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
        return;
    }

    FDialogueRowBase* row = MyDataTable->FindRow<FDialogueRowBase>(FName(ItemName), ContextString);

    if (!row)
    {
        UE_LOG(LogTemp, Warning, TEXT("Row not found in DataTable"));
        return;
    }

    if (DialogueTextBlock)
    {
        DialogueTextBlock->SetText(FText::FromString(row->source));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("DialogueTextBlock is null"));
        return;
    }

    if (DialogueWidget)
    {
        DialogueWidget->SetVisibility(ESlateVisibility::Visible);
    }

    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("Owner is not of class APanaceaCharacter"));
        return;
    }

    Character->TriggerSoundPlay(FName(row->SoundToPlay));

    // Use a weak pointer to avoid potential crashes if the actor is destroyed
    TWeakObjectPtr<ADialogueManagerActor> WeakThis(this);
    GetWorld()->GetTimerManager().SetTimer(DialogueVisibilityTimerHandle, [WeakThis]()
        {
            if (WeakThis.IsValid())
            {
                if (WeakThis->DialogueWidget)
                {
                    WeakThis->DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
                }
            }
        }, TimeShow, false);
}