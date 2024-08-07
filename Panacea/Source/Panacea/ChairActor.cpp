// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairActor.h"
#include "SwitchComponent.h"
#include "GameFramework/Character.h" 
#include "InteractiveComponent.h"

AChairActor::AChairActor()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and initialize StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Create and initialize SwitchComponent
	SwitchComponent = CreateDefaultSubobject<USwitchComponent>(TEXT("SwitchComponent"));
	SwitchComponent->SetupAttachment(RootComponent);

	MinimumDistance = 20.0f;
}

void AChairActor::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = StaticMeshComponent->GetComponentLocation();
}

void AChairActor::Interact()
{
	if (!Interactable)
		return;

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetSimulatePhysics(!StaticMeshComponent->IsSimulatingPhysics());
	}

	if (SwitchComponent)
	{
		SwitchComponent->SwitchCamera();
	}

	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UInteractiveComponent* InteractiveComponent = Character->GetComponentByClass<UInteractiveComponent>();

	if (InteractiveComponent)
	{
		InteractiveComponent->bIsHolding = !InteractiveComponent->bIsHolding;
	}

	FVector FinalLocation = StaticMeshComponent->GetComponentLocation();

	float Distance = FVector::Distance(OriginalLocation, FinalLocation);

	UE_LOG(LogTemp, Warning, TEXT("Distance Difference : %f"), Distance);


	// Check if the angle difference is within the desired threshold (e.g., 23 degrees)
	if (Distance > MinimumDistance)
	{
		Broadcast();
		SetNotInteractable();

		if (InteractiveComponent)
		{
			InteractiveComponent->ResetActorInFocus(this);
		}
	}
}
