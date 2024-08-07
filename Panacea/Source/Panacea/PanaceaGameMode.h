// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PanaceaGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIngredientAdded, const FString&, IngredientName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBadEnding);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemInteracted, const FString&, ItemName);

UCLASS(minimalapi)
class APanaceaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APanaceaGameMode();

	UPROPERTY(BlueprintAssignable, Category = "Ingredients")
	FOnIngredientAdded OnIngredientAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "Endings")
	FOnBadEnding OnBadEnding;

	UPROPERTY(BlueprintAssignable)
	FOnItemInteracted OnItemInteractedDelegate;
	// The widget class to use for good ending
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GoodEndingWidgetClass;

	// The widget instance of good ending widget
	UPROPERTY()
	UUserWidget* GoodEndingWidgetInstance;

	// The widget class to use for bad ending
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> BadEndingWidgetClass;

	// The widget instance of bad ending widget
	UPROPERTY()
	UUserWidget* BadEndingWidgetInstance;

	// Function to record the ingredient
	UFUNCTION()
	void RecordIngredient(const FString& IngredientName);

	// Function to record the ingredient
	UFUNCTION()
	void RecordItemInteraction(const FString& ItemName);

	// Function to start Bad ending sequence
	UFUNCTION()
	void OnBadEndingSequence();

	UFUNCTION(BlueprintCallable)
	void BroadcastBadEndingEvent();
	
	UFUNCTION(BlueprintCallable)
	void BroadcastOnItemInteracted(const FString& IngredientName);

	TArray<FString> GetItemNames() const { return ItemNames; }

private:

	void CheckGoodEnding();
	
	bool CheckBadEnding(const FString& IngredientName);
	
	TArray<FString> IngredientNames;

	TArray<FString> ItemNames;

};



