// Fill out your copyright notice in the Description page of Project Settings.

#include "SwitchComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MouseDragObjectsComponent.h"

// Sets default values for this component's properties
USwitchComponent::USwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Create and configure the camera component
	ObjectCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ObjectCamera"));

	// Ensure ObjectCamera is properly attached to a scene component
	// For this example, we'll attach it to the owner’s root component
	if (GetOwner())
	{
		USceneComponent* Root = GetOwner()->GetRootComponent();
		if (Root)
		{
			ObjectCamera->SetupAttachment(Root); // Attach to root component
		}
	}

	ObjectCamera->SetRelativeLocation(FVector(-78.f, 10.f, 160.f)); // Adjust as needed
	ObjectCamera->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // Adjust as needed

	OriginalViewTarget = nullptr;
}


// Called when the game starts
void USwitchComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectCamera)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Camera Found"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(
				TEXT("Camera Location: %s"),
				*ObjectCamera->GetRelativeLocation().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
			FString::Printf(
				TEXT("Camera Rotation: %s"),
				*ObjectCamera->GetRelativeRotation().ToString()));
	}
}


void USwitchComponent::SwitchCamera()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerController not found"));
		return;
	}

	if (PlayerController->GetViewTarget() == GetOwner())
	{
		if (OriginalViewTarget)
		{
			PlayerController->SetViewTargetWithBlend(OriginalViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			OriginalViewTarget = nullptr;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Switched back to original view"));
		}
	}
	else
	{
		OriginalViewTarget = PlayerController->GetViewTarget();
		PlayerController->SetViewTargetWithBlend(GetOwner(), 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Switched to object camera view"));
	}

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UMouseDragObjectsComponent* MouseDragObjectsComponent = Character->GetComponentByClass<UMouseDragObjectsComponent>();
	
	if (MouseDragObjectsComponent)
	{
		MouseDragObjectsComponent->SwitchGrabMode();
	}
}

void USwitchComponent::SetupAttachment(TObjectPtr<USceneComponent> Object)
{
	ObjectCamera->SetupAttachment(Object);
}
