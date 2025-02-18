// Fill out your copyright notice in the Description page of Project Settings.


#include "AlchemyCircleActor.h"

#include "InteractiveComponent.h"
#include "EngineUtils.h" 
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MetasoundSource.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

AAlchemyCircleActor::AAlchemyCircleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsGlowing = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and initialize StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Create and set up a sphere component as the root component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);

	// Set up the collision parameters
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAlchemyCircleActor::OnOverlapBegin);

	//draw  debug sphere
	CollisionComponent->SetSphereRadius(50.0f);

	MetaSoundAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MetaSoundAudioComponent"));
	MetaSoundAudioComponent->SetupAttachment(RootComponent);
}


void AAlchemyCircleActor::BeginPlay()
{
	Super::BeginPlay();


	ACharacter* TempCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!TempCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Player character is null in AlchemyCircleActor."));
	}

	PlayerCharacter = Cast<APanaceaCharacter>(TempCharacter);

	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null in AlchemyCircleActor"));
	}

	InteractiveComponent = PlayerCharacter->FindComponentByClass<UInteractiveComponent>();
	if (!InteractiveComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("InteractiveComponent is null in AlchemyCircleActor"));
	}


	if (MetaSoundSource)
	{
		MetaSoundAudioComponent->SetSound(MetaSoundSource);
	}
}

void AAlchemyCircleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	AItem* Ingredient = Cast<AItem>(OtherActor);
	if (!Ingredient)
		return;


	if (IngredientNameToPlace == Ingredient->GetActorNameOrLabel())
	{
		if (!Ingredient->IsAttachedTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			Ingredient->SetNotInteractable();
			InteractiveComponent->HideActor(Ingredient);
			Broadcast();

			if (NiagaraSystem)
			{
				UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, DefaultSceneRoot, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
			}

			if (MetaSoundAudioComponent)
			{
				MetaSoundAudioComponent->Play();
			}
		}
	}
}

void AAlchemyCircleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractiveComponent &&
		InteractiveComponent->bIsHolding &&
		InteractiveComponent->GetActorInFocus()->GetActorNameOrLabel() == IngredientNameToPlace)
	{

		if (bIsGlowing)
			return;

		StaticMeshComponent->SetRenderCustomDepth(true);

		bIsGlowing = true;

	}
	else
	{
		if (!bIsGlowing)
			return;

		StaticMeshComponent->SetRenderCustomDepth(false);

		bIsGlowing = false;

	}
}
