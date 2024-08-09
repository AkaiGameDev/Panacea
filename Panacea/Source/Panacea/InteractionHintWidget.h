// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionHintWidget.generated.h"

/**
 * 
 */
UCLASS()
class PANACEA_API UInteractionHintWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="InteracionHint")
	void SetTextOfInteractionHint(FText InText);

	UFUNCTION(BlueprintCallable,Category="InteractionHint")
	FText GetTextOfInteractionHint() const;

protected:
	UPROPERTY(BlueprintReadOnly,Category="InteractionHint")
	FText InteractionHintText;
	
};
