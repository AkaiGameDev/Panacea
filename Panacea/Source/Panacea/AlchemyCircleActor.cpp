// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyCircleActor.h"

#include "InteractiveComponent.h"

AAlchemyCircleActor::AAlchemyCircleActor()
{
	// Create and set up a sphere component as the root component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	// Set up the collision parameters
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAlchemyCircleActor::OnOverlapBegin);

	//draw  debug sphere
	CollisionComponent->SetSphereRadius(50.0f);
}


void AAlchemyCircleActor::BeginPlay()
{
	Super::BeginPlay();


	ACharacter* TempCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!TempCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Player character is null in AlchemyCircleActor."));
	}

	PlayerCharacter = Cast<APanaceaCharacter>(TempCharacter);

	if (PlayerCharacter)
	{
		InteractiveComponent = PlayerCharacter->FindComponentByClass<UInteractiveComponent>();
		if (!InteractiveComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("InteractiveComponent is null in AlchemyCircleActor"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null in AlchemyCircleActor"));
	}
}

void AAlchemyCircleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	AItem* Ingredient = Cast<AItem>(OtherActor);
	if (!Ingredient)
		return;


	if (IngredientNameToPlace == Ingredient->GetActorNameOrLabel())
	{
		if (InteractiveComponent && (InteractiveComponent->bIsHolding))
		{
			Ingredient->SetNotInteractable();
			InteractiveComponent->HideActor(Ingredient);
			Broadcast();
		}
	}
}
