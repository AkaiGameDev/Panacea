// Copyright Epic Games, Inc. All Rights Reserved.

#include "PanaceaGameMode.h"
#include "PanaceaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

APanaceaGameMode::APanaceaGameMode()
	: Super()
{
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	OnIngredientAdded.AddDynamic(this, &APanaceaGameMode::RecordIngredient);
	OnBadEnding.AddDynamic(this, &APanaceaGameMode::OnBadEndingSequence);
	OnItemInteractedDelegate.AddDynamic(this, &APanaceaGameMode::RecordItemInteraction); //change this to a new function to record items interacted w
}

void APanaceaGameMode::RecordIngredient(const FString& IngredientName)
{
	IngredientNames.Add(IngredientName);

	UE_LOG(LogTemp, Warning, TEXT("Ingredient Added: %s"), *IngredientName);

	if (CheckBadEnding(IngredientName))
	{
		OnBadEnding.Broadcast();
	}
	else
		CheckGoodEnding();
}

void APanaceaGameMode::RecordItemInteraction(const FString& ItemName)
{
	ItemNames.Add(ItemName);


	for (auto Item : ItemNames)
	{

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Item);
	}

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
	if (IngredientNames.Contains("Amber_AlchemyCircle") && IngredientNames.Contains("Hair_AlchemyCircle") && IngredientNames.Contains("Mushroom_AlchemyCircle"))
	{
		if (GoodEndingWidgetClass)
		{
			// Create the widget and add it to the viewport
			GoodEndingWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GoodEndingWidgetClass);
			if (GoodEndingWidgetInstance)
			{
				GoodEndingWidgetInstance->AddToViewport();
			}
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
