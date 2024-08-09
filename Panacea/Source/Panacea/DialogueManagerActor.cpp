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

}


void ADialogueManagerActor::ShowDialogue(const FString& ItemName)
{
	static const FString ContextString(TEXT("GENERAL"));

	if (MyDataTable)
	{
		FDialogueRowBase* row = MyDataTable->FindRow<FDialogueRowBase>(FName(ItemName), ContextString);

		if (!row)
			return;

		DialogueTextBlock->SetText(FText::FromString(row->source));

		if (DialogueWidget)
			DialogueWidget->SetVisibility(ESlateVisibility::Visible);

		GetWorld()->GetTimerManager().SetTimer(DialogueVisibilityTimerHandle, [this]()
			{
				if (DialogueWidget)
				{
					DialogueWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}, 5.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is null"));
	}
}