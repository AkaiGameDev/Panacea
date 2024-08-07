// Fill out your copyright notice in the Description page of Project Settings.


#include "CrossActor.h"
#include "SwitchComponent.h"
#include "GameFramework/Character.h" 
#include "InteractiveComponent.h"

ACrossActor::ACrossActor()
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

	MinimumDegrees = 120.0f;
}

void ACrossActor::BeginPlay()
{
	Super::BeginPlay();

	OriginalAngleVector = StaticMeshComponent->GetComponentRotation().Vector();
}

void ACrossActor::Interact()
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

	FVector FinalAngleVector = StaticMeshComponent->GetComponentRotation().Vector();

	// Normalize the vectors
	OriginalAngleVector.Normalize();
	FinalAngleVector.Normalize();

	// Calculate the dot product of the normalized vectors
	float DotProduct = FVector::DotProduct(OriginalAngleVector, FinalAngleVector);

	// Compute the angle difference in radians
	float AngleDifferenceRadians = FMath::Acos(DotProduct);

	// Convert radians to degrees if needed
	float AngleDifferenceDegrees = FMath::RadiansToDegrees(AngleDifferenceRadians);

	UE_LOG(LogTemp, Warning, TEXT("Angle Difference (Degrees): %f"), AngleDifferenceDegrees);

	// Check if the angle difference is within the desired threshold (e.g., 23 degrees)
	if (AngleDifferenceDegrees > MinimumDegrees)
	{
		Broadcast();
		SetNotInteractable();

		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		UInteractiveComponent* InteractiveComponent = Character->GetComponentByClass<UInteractiveComponent>();

		if (InteractiveComponent)
		{
			InteractiveComponent->ResetActorInFocus(this);
		}
	}
}


//void ACrossActor::Tick(float DeltaTime)
//{
//	FVector FinalAngleVector = StaticMeshComponent->GetComponentRotation().Vector();
//
//	// Normalize the vectors
//	OriginalAngleVector.Normalize();
//	FinalAngleVector.Normalize();
//
//	// Calculate the dot product of the normalized vectors
//	float DotProduct = FVector::DotProduct(OriginalAngleVector, FinalAngleVector);
//
//	// Compute the angle difference in radians
//	float AngleDifferenceRadians = FMath::Acos(DotProduct);
//
//	// Convert radians to degrees if needed
//	float AngleDifferenceDegrees = FMath::RadiansToDegrees(AngleDifferenceRadians);
//
//	UE_LOG(LogTemp, Warning, TEXT("Angle Difference (Degrees): %f"), AngleDifferenceDegrees);
//
//}
