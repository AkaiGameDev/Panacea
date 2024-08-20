// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteWidget.generated.h"

class ANoteActor;

UCLASS()
class PANACEA_API UNoteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Sets the Note Actor reference */
	UFUNCTION(BlueprintCallable, Category = "Note")
	void SetNoteActor(ANoteActor* InNoteActor);

	/** Gets the Note Actor reference */
	UFUNCTION(BlueprintCallable, Category = "Note")
	ANoteActor* GetNoteActor() const;

	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UTexture2D* NoteImage;

protected:
	/** Note actor reference */
	UPROPERTY(BlueprintReadOnly, Category = "Note")
	ANoteActor* NoteActor;
};
