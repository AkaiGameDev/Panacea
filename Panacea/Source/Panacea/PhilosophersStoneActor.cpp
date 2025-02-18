// Fill out your copyright notice in the Description page of Project Settings.


#include "PhilosophersStoneActor.h"
#include "Blueprint/UserWidget.h"

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

	APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
		return;
	}

	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameMode->OnItemInteractedDelegate.AddDynamic(this, &APhilosophersStoneActor::NoteRead);
	//GameMode->OnItemInteractedDelegate.AddDynamic(this, &AItem::CheckInteractable);
	//Enable();
}

// Called every frame
void APhilosophersStoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEnabled)
	{
		MoveStone(DeltaTime);
		if(HasReadNote)
			StoneMeshComponent->SetRenderCustomDepth(true);
	}
}

void APhilosophersStoneActor::Interact()
{
	if (HasReadNote) {
		if (!EndingCutSceneWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("EndingCutSceneWidgetClass is null"));
			return;
		}

		EndingCutSceneWidget = CreateWidget<UUserWidget>(GetWorld(), EndingCutSceneWidgetClass);
		if (EndingCutSceneWidget)
		{
			EndingCutSceneWidget->AddToViewport(0);
		}
	} else
		Broadcast();
}

void APhilosophersStoneActor::Broadcast()
{
	UE_LOG(LogTemp, Warning, TEXT("Enters broadcast"));
	APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is null"));
		return;
	}
	if (HasReadNote) {
		FString ItemID = GetActorNameOrLabel();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemID);
		bool bIsFirst = !GameMode->GetItemNames().Contains(ItemID);
		if (bIsFirst)
			GameMode->OnItemInteractedDelegate.Broadcast(ItemID);
	}
	else {
		FString ReadNoteSignal = "ReadNoteFirst";
		GameMode->OnItemInteractedDelegate.Broadcast(ReadNoteSignal);
	}
}

void APhilosophersStoneActor::Enable()
{
	bEnabled = true;
	StoneMeshComponent->SetVisibility(true);
	SetInteractable();
}

void APhilosophersStoneActor::MoveStone(float DeltaTime)
{
	if (bHasReachedDestination)
		return;

	FVector NewLocation = GetActorLocation();
	NewLocation.Z += Speed * DeltaTime;
	SetActorLocation(NewLocation);

	if ((GetActorLocation().Z - InitialLocation.Z) > MaxHeight)
	{
		bHasReachedDestination = true;
	}
}

void APhilosophersStoneActor::NoteRead(const FString& itemInteracted)
{
	if (itemInteracted == InteractableTrigger) {
		UE_LOG(LogTemp, Warning, TEXT("Stone set to NoteRead"));
		HasReadNote = true;
		InteractionHintText = "\"E\" to complete stone";
		StoneMeshComponent->SetRenderCustomDepth(true);
	}
}
