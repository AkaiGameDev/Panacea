// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyActor.h"
#include "GameFramework/Character.h" 
#include "InteractiveComponent.h"

void AKeyActor::Interact()
{
	FirstInteraction();
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UInteractiveComponent* InteractiveComponent = Character->GetComponentByClass<UInteractiveComponent>();
	if (InteractiveComponent)
	{
		if (InteractiveComponent->bIsHolding)
			InteractiveComponent->GrabbedActorRotationViewport = FRotator::ZeroRotator;
		else
			InteractiveComponent->GrabbedActorRotationViewport = FRotator(0.0f, 90.0f, 90.0f);

	}


	bool bIsChestOverlapping = false;

	TArray<AActor*> OverlappingActors;
	Character->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->ActorHasTag("Chest"))
		{
			bIsChestOverlapping = true;
		}
	}

	if (bIsChestOverlapping)
	{
		Broadcast();

		if (InteractiveComponent)
		{
			InteractiveComponent->SetAndStartMovement(ChestLockLocation, ChestLockRotation, true);
			SetNotInteractable();
		}
	}
}
