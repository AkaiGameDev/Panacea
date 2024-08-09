// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoteActor.h"
#include "DiaryActor.generated.h"

/**
 * 
 */
UCLASS()
class PANACEA_API ADiaryActor : public ANoteActor
{
	GENERATED_BODY()

public: 
	//text
	UPROPERTY(EditAnywhere, Category = "UI")
	FString PageOneText;

	UPROPERTY(EditAnywhere, Category = "UI")
	FString PageTwoText;

	virtual void OpenNote() override;
};
