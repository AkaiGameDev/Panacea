// Fill out your copyright notice in the Description page of Project Settings.


#include "CauldronCollisionSphereActor.h"

#include "Engine/Engine.h"
#include "PanaceaGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACauldronCollisionSphereActor::ACauldronCollisionSphereActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up a sphere component as the root component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	// Set up the collision parameters
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACauldronCollisionSphereActor::OnOverlapBegin);





}

// Called when the game starts or when spawned
void ACauldronCollisionSphereActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACauldronCollisionSphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACauldronCollisionSphereActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Something collided with the cauldron!"));

	if (OtherActor->Tags.Num() == 0)
	{
		return;
	}

	FString OtherActorTag = OtherActor->Tags[0].ToString();
	if (OtherActorTag == "Ingredient")
	{
		FTimerHandle TimerHandle;

		GetWorldTimerManager().SetTimer(TimerHandle, [this, OtherActor]()
		{
			OnIngredientAdded(OtherActor);
		}, 1.0f, false);
	}
}


void ACauldronCollisionSphereActor::OnIngredientAdded(AActor* Ingredient)
{
	APanaceaGameMode* GameMode = Cast<APanaceaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->OnIngredientAdded.Broadcast(Ingredient->GetActorNameOrLabel());
	}

	Ingredient->Destroy();
}
