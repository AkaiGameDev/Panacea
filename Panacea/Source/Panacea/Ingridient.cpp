// Fill out your copyright notice in the Description page of Project Settings.


#include "Ingridient.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "SwitchComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIngridient::AIngridient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//add tag Ingredient o the actor
	Tags.Add("Ingredient");

	// Create and initialize StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// Create and initialize SwitchComponent
	SwitchComponent = CreateDefaultSubobject<USwitchComponent>(TEXT("SwitchComponent"));
	SwitchComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AIngridient::BeginPlay()
{
	Super::BeginPlay();

	// Ensure StaticMeshComponent is initialized
	if (StaticMeshComponent)
	{
		TArray<USceneComponent*> ChildComponents;
		StaticMeshComponent->GetChildrenComponents(true, ChildComponents);

		for (USceneComponent* Child : ChildComponents)
		{
			// Check if the child is a UStaticMeshComponent
			UStaticMeshComponent* ChildMesh = Cast<UStaticMeshComponent>(Child);
			if (ChildMesh)
			{
				// Log the name of the found mesh
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Child Mesh Found: ") + ChildMesh->GetName());
				StaticMeshComponent = ChildMesh;
				break; // Exit the loop after finding the first child mesh
			}
		}
	}

	if (!StaticMeshComponent)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No child StaticMeshComponent found"));
	}



}

// Called every frame
void AIngridient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIngridient::Interact()
{
	//call the switch camera component to switch the camera
	if (SwitchComponent)
	{
		SwitchComponent->SwitchCamera();
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SwitchComponent not found"));
	}
}

void AIngridient::OnInteractableInRange()
{
	StaticMeshComponent->SetRenderCustomDepth(true);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("In Range:") + GetActorNameOrLabel());
}

void AIngridient::OnInteractableOutOfRange()
{
	StaticMeshComponent->SetRenderCustomDepth(false);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Out Of Range:") + GetActorNameOrLabel());
}

void AIngridient::SetMaterial(UMaterialInterface* NewMaterial)
{
	if (NewMaterial && StaticMeshComponent)
	{
		StaticMeshComponent->SetMaterial(0, NewMaterial); // 0 is the index for the first material slot
	}
}