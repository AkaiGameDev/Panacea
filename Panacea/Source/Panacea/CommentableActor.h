// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "CommentableActor.generated.h"

/**
 * 
 */
UCLASS()
class PANACEA_API ACommentableActor : public AItem
{
	GENERATED_BODY()


public:
	virtual void Interact() override;
};
