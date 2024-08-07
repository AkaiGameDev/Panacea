#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "NoteActor.generated.h"

UCLASS()
class PANACEA_API ANoteActor : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANoteActor();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	//text
	UPROPERTY(EditAnywhere, Category= "UI")
	FString NoteText;

	UPROPERTY()
	UUserWidget* PauseMenuWidget = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact() override;
};
