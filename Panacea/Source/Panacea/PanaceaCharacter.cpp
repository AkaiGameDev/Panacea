// Copyright Epic Games, Inc. All Rights Reserved.

#include "PanaceaCharacter.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GrabbingSystemComponent.h"
#include "MouseDragObjectsComponent.h"
#include "InteractiveComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APanaceaCharacter

APanaceaCharacter::APanaceaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	MouseDragObjectsComponent = CreateDefaultSubobject<UMouseDragObjectsComponent>(TEXT("MouseDragObjectsComponent"));

	// Create an InteractiveComponent
	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));

}

void APanaceaCharacter::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("APanaceaCharacter::BeginPlay - Start"));
	if (CrosshairWidgetClass) // Ensure the widget class is set
	{
		if (CrosshairWidgetClass)
		{
			CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("Crosshair widget created"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create Crosshair widget"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CrosshairWidgetClass is null"));
		}
	}

	if (HintInteractionWidgetClass)
	{
		HintInteractionWidget = CreateWidget<UUserWidget>(GetWorld(), HintInteractionWidgetClass);

		if (HintInteractionWidget)
		{
			HintInteractionWidget->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("Hint Interaction widget created"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Hint Interaction widget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HintInteractionWidgetClass is null"));
	}

	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get Enhanced Input Local Player Subsystem"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is not a PlayerController"));
	}

	UE_LOG(LogTemp, Warning, TEXT("APanaceaCharacter::BeginPlay - End"));
}

//////////////////////////////////////////////////////////////////////////// Input

void APanaceaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	if (!PlayerInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInputComponent is null"));
		return;
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APanaceaCharacter::Move);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APanaceaCharacter::Look);

		//Reset Sequence
		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Triggered, this,
		                                   &APanaceaCharacter::OnRestart);

		//Pause action
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APanaceaCharacter::Pause);

		//Interact action
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		                                   &APanaceaCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

UUserWidget* APanaceaCharacter::GetCrosshairWidget() const
{
	return CrosshairWidget;
}

UUserWidget* APanaceaCharacter::GetHintInteractionWidget() const
{
	return HintInteractionWidget;
}


void APanaceaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APanaceaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APanaceaCharacter::OnRestart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APanaceaCharacter::Pause()
{
	UE_LOG(LogTemp, Log, TEXT("Pause works?"));
}

void APanaceaCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractiveComponent)
		InteractiveComponent->Interact(Value);
}
