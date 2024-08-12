// Fill out your copyright notice in the Description page of Project Settings.


#include "CommentableActor.h"


void ACommentableActor::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Commentable Actor"));
	Interactable = false;
	Broadcast();
}