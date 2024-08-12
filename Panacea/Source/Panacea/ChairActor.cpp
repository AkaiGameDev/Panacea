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

	StaticMeshComponent->SetWorldLocation(StaticMeshComponent->GetComponentLocation() - StaticMeshComponent->GetForwardVector() * MinimumDistance);

	OriginalLocation = StaticMeshComponent->GetComponentLocation();

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Character)
		return;

	InteractiveComponent = Character->GetComponentByClass<UInteractiveComponent>();
}

void AChairActor::Tick(float DeltaTime)
{
	if (!InteractiveComponent ||
		InteractiveComponent->GetActorInFocus() != this ||
		!InteractiveComponent->bIsHolding)
		return;

	FVector FinalLocation = StaticMeshComponent->GetComponentLocation();

	float Distance = FVector::Distance(OriginalLocation, FinalLocation);

	UE_LOG(LogTemp, Warning, TEXT("Distance Difference : %f"), Distance);


	if (Distance > MinimumDistance)
	{
		Interact();
		Broadcast();
		SetNotInteractable();

		if (InteractiveComponent)
		{
			InteractiveComponent->ResetActorInFocus(this);
		}
	}

}


void AChairActor::Interact()
{
	if (!Interactable)
		return;

	if (StaticMeshComponent)
	{
		if (!StaticMeshComponent->IsSimulatingPhysics())
		{
			StaticMeshComponent->SetSimulatePhysics(true);
			StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

		}
		else
		{
			StaticMeshComponent->SetSimulatePhysics(false);
			StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Block);
		}
	}

	if (SwitchComponent)
	{
		SwitchComponent->SwitchCamera();
	}

	if (InteractiveComponent)
	{
		InteractiveComponent->bIsHolding = !InteractiveComponent->bIsHolding;
	}
}
