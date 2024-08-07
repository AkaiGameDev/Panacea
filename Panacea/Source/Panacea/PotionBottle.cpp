#include "PotionBottle.h"
#include "MouseDragObjectsComponent.h"
#include "SwitchComponent.h"
#include "InteractiveComponent.h"

APotionBottle::APotionBottle()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsBreaked = false;
	BreakableDistance = 20.0f;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and initialize StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	GeometryCollectionComponent->SetupAttachment(RootComponent);
	GeometryCollectionComponent->SetVisibility(false);
	GeometryCollectionComponent->SetSimulatePhysics(false);

	SwitchComponent = CreateDefaultSubobject<USwitchComponent>(TEXT("SwitchComponent"));
	SwitchComponent->SetupAttachment(RootComponent);
}

void APotionBottle::BeginPlay()
{
	Super::BeginPlay();

	if (GeometryCollectionComponent)
	{
		// Bind to the fracture event
		GeometryCollectionComponent->SetNotifyBreaks(true);
		GeometryCollectionComponent->OnChaosBreakEvent.AddDynamic(this, &APotionBottle::OnComponentFracture);
	}

	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UMouseDragObjectsComponent* MouseDragObjectsComponent = Character->GetComponentByClass<UMouseDragObjectsComponent>();

	if (MouseDragObjectsComponent)
	{
		MouseDragObjectsComponent->OnComponentMouseRelease.AddDynamic(this, &APotionBottle::OnComponentReleased);
	}
}

void APotionBottle::OnComponentFracture(const FChaosBreakEvent& BreakEvent)
{
	if (!bIsBreaked)
	{
		bIsBreaked = true;

		

		Interact();

		Broadcast();
		SetNotInteractable();

		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		UInteractiveComponent* InteractiveComponent = Character->GetComponentByClass<UInteractiveComponent>();

		if (InteractiveComponent)
		{
			InteractiveComponent->ResetActorInFocus(this);

			if (ActorToSpawn)
			{

				// Get the location and rotation of the current actor
				FVector SpawnLocation = BreakEvent.Location;
				FRotator SpawnRotation = GetActorRotation();

				// Spawn the new actor
				AItem* SpawnedActor = GetWorld()->SpawnActor<AItem>(ActorToSpawn, SpawnLocation, SpawnRotation);
				
				if(SpawnedActor)
				{
					SpawnedActor->SetActorLabel("Amber");

					UStaticMeshComponent* MeshComponent = SpawnedActor->FindComponentByClass<UStaticMeshComponent>();
					if (MeshComponent)
					{
						FVector Origin, BoxBounds;
						MeshComponent->GetLocalBounds(Origin, BoxBounds);
						FVector ActorScale = MeshComponent->GetComponentScale();
						BoxBounds = BoxBounds * ActorScale;
						SpawnedActor->SetActorLocation(SpawnLocation + FVector::UpVector * BoxBounds.Z * 0.5f);
					}

					SpawnedActor->Interactable = true;

					/*APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					if (GameMode)
					{
						GameMode->OnIngredientAdded.Broadcast(SpawnedActor->GetActorNameOrLabel());
					}*/
				}

			}
		}
	}
}

void APotionBottle::OnComponentReleased(UPrimitiveComponent* ReleasedComponent)
{
	if (!Interactable) {
		return;
	}

	FVector Start = StaticMeshComponent->GetComponentLocation();
	FVector End = Start - FVector(0.0f, 0.0f, BreakableDistance); 

	FVector Origin, BoxBounds;
	StaticMeshComponent->GetLocalBounds(Origin, BoxBounds);
	FVector ActorScale = StaticMeshComponent->GetComponentScale();
	FVector BoxExtent = BoxBounds * ActorScale; 

	FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);
	FHitResult HitResult;


	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Character);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		Box,
		QueryParams
	);

	/*DrawDebugBox(GetWorld(), Start, BoxExtent, FColor::Red, false, 1.0f);
	DrawDebugBox(GetWorld(), End, BoxExtent, FColor::Green, false, 1.0f);
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.0f, 0, 1.0f);*/

	if (!bHit)
	{
		FTransform MeshTransform = StaticMeshComponent->GetComponentTransform();
		StaticMeshComponent->DestroyComponent();

		GeometryCollectionComponent->SetWorldTransform(MeshTransform);
		GeometryCollectionComponent->SetVisibility(true);
		GeometryCollectionComponent->SetSimulatePhysics(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Potion bottle is too low to break."));
	}
}

void APotionBottle::Interact()
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

	FirstInteraction();
}

