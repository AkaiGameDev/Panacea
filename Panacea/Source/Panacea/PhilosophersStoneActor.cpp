// Fill out your copyright notice in the Description page of Project Settings.


#include "PhilosophersStoneActor.h"

// Sets default values
APhilosophersStoneActor::APhilosophersStoneActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APhilosophersStoneActor::BeginPlay()
{
	Super::BeginPlay();


	StoneMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));

	if (!StoneMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No mesh component found"));
		return;
	}

	InitialLocation = GetActorLocation();
	StoneMeshComponent->SetVisibility(false);

	Enable();
}

// Called every frame
void APhilosophersStoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bEnabled)
	{
		MoveStone(DeltaTime);
	}
}

void APhilosophersStoneActor::Enable()
{
	bEnabled = true;
	StoneMeshComponent->SetVisibility(true);
	Interactable = true;
}

void APhilosophersStoneActor::MoveStone(float DeltaTime)
{

	if ((GetActorLocation().Z - InitialLocation.Z) > MaxHeight)
	{
		return;
	}

	FVector NewLocation = GetActorLocation();
	NewLocation.Z += Speed * DeltaTime;
	SetActorLocation(NewLocation);
}
