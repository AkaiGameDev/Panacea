// Copyright Epic Games, Inc. All Rights Reserved.

#include "PanaceaGameMode.h"
#include "PanaceaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PhilosophersStoneActor.h"

APanaceaGameMode::APanaceaGameMode()
	: Super()
{
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	
}


void APanaceaGameMode::BeginPlay()
{
	Super::BeginPlay();

	OnItemInteractedDelegate.AddDynamic(this, &APanaceaGameMode::RecordItemInteraction); //change this to a new function to record items interacted w
	OnBadEnding.AddDynamic(this, &APanaceaGameMode::OnBadEndingSequence);
}

void APanaceaGameMode::RecordItemInteraction(const FString& ItemName)
{
	ItemNames.Add(ItemName);

	UE_LOG(LogTemp, Warning, TEXT("WORKS"));

	for (auto Item : ItemNames)
	{

		UE_LOG(LogTemp, Warning, TEXT("ITEMS: %s"), *Item);
	}

	CheckGoodEnding();
}

void APanaceaGameMode::OnBadEndingSequence()
{
	if (BadEndingWidgetClass)
	{
		// Create the widget and add it to the viewport
		BadEndingWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), BadEndingWidgetClass);
		if (BadEndingWidgetInstance)
		{
			BadEndingWidgetInstance->AddToViewport();
		}
	}
}

void APanaceaGameMode::BroadcastBadEndingEvent()
{
	OnBadEnding.Broadcast();
}

void APanaceaGameMode::BroadcastOnItemInteracted(const FString& IngredientName)
{
	OnItemInteractedDelegate.Broadcast(IngredientName);
}

void APanaceaGameMode::CheckGoodEnding()
{
	if (ItemNames.Contains("Amber_AlchemyCircle") && ItemNames.Contains("Hair_AlchemyCircle") && ItemNames.Contains("Mushroom_AlchemyCircle"))
	{
		APhilosophersStoneActor* PhilosophersStone = Cast<APhilosophersStoneActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APhilosophersStoneActor::StaticClass()));
		if (PhilosophersStone)
		{
			PhilosophersStone->Enable();
		}
	}
}

bool APanaceaGameMode::CheckBadEnding(const FString& IngredientName)
{
	if (IngredientName == "WrongIngredient")
		return true;
	else
		return false;
}
