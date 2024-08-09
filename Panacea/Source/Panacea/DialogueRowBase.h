// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueRowBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogueRowBase : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString item_trigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString source;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

};
