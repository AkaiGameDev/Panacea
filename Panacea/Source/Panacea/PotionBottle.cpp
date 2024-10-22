#include "PotionBottle.h"
#include "MouseDragObjectsComponent.h"
#include "SwitchComponent.h"
#include "InteractiveComponent.h"
#include "MetasoundSource.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

APotionBottle::APotionBottle()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsBreaked = false;
	bHasAmber = false;
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

	MetaSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MetaSoundAudioComponent"));
	MetaSoundAudioComponent->SetupAttachment(RootComponent);

	CheckBoxOnTickX = 20.0f;
	CheckBoxOnTickY = 20.0f;
	CheckBoxX = -17.0f;
	CheckBoxY = -17.0f;
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
	if (!Character)
		return;

	UMouseDragObjectsComponent* MouseDragObjectsComponent = Character->GetComponentByClass<UMouseDragObjectsComponent>();

	if (MouseDragObjectsComponent)
	{
		MouseDragObjectsComponent->OnComponentMouseRelease.AddDynamic(this, &APotionBottle::OnComponentReleasedTrigger);
	}

	InteractiveComponent = Character->GetComponentByClass<UInteractiveComponent>();

	if (MetaSoundSource)
	{
		MetaSoundAudioComponent->SetSound(MetaSoundSource);
	}
}

void APotionBottle::Tick(float DeltaTime)
{
	if (!InteractiveComponent)
		return;

	if (InteractiveComponent->GetActorInFocus() != this)
		return;

	if (!InteractiveComponent->bIsHolding)
		return;

	OnComponentReleased(StaticMeshComponent, true);
}

void APotionBottle::OnComponentFracture(const FChaosBreakEvent& BreakEvent)
{
	if (!bIsBreaked)
	{
		bIsBreaked = true;

		Interact();
		SetNotInteractable();

		if (InteractiveComponent)
		{
			InteractiveComponent->ResetActorInFocus(this);


			if (MetaSoundAudioComponent)
			{
				MetaSoundAudioComponent->Play();
			}

			if (bHasAmber && ActorToSpawn)
			{
				Broadcast();
				// Get the location and rotation of the current actor
				FVector SpawnLocation = BreakEvent.Location;
				FRotator SpawnRotation = GetActorRotation();
				FActorSpawnParameters SpawnParams;
				SpawnParams.Name = FName(TEXT("Amber"));
				// Spawn the new actor
				AItem* SpawnedActor = GetWorld()->SpawnActor<AItem>(ActorToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

				if (SpawnedActor)
				{
#if WITH_EDITOR
					SpawnedActor->SetActorLabel(TEXT("Amber"));
#endif

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

				}

			}
		}
	}
}

void APotionBottle::OnComponentReleased(UPrimitiveComponent* ReleasedComponent, bool bOnTick /*= false*/)
{
	if (!Interactable) {
		return;
	}

	FVector Origin, BoxBounds;
	StaticMeshComponent->GetLocalBounds(Origin, BoxBounds);
	FVector ActorScale = StaticMeshComponent->GetComponentScale();
	FVector BoxExtent = BoxBounds * ActorScale;

	if (bOnTick) {
		BoxExtent.X += CheckBoxOnTickX;
		BoxExtent.Y += CheckBoxOnTickY;
	}
	else
	{
		BoxExtent.X += CheckBoxY;
		BoxExtent.Y += CheckBoxY;
	}

	BoxExtent.Z /= 2.0f;

	FCollisionShape Box = FCollisionShape::MakeBox(BoxExtent);
	FHitResult HitResult;

	FVector Start = StaticMeshComponent->GetComponentLocation();
	Start.Z += BoxExtent.Z;
	FVector End = Start - FVector(0.0f, 0.0f, BreakableDistance);

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

void APotionBottle::OnComponentReleasedTrigger(UPrimitiveComponent* ReleasedComponent)
{
	OnComponentReleased(ReleasedComponent);
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


	if (InteractiveComponent)
	{
		InteractiveComponent->bIsHolding = !InteractiveComponent->bIsHolding;
	}

	FirstInteraction();
}

