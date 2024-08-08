#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
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

	bool bOpened = false;

	UPROPERTY()
	ACharacter* PlayerCharacter;

	static TArray<ANoteActor*> NoteActors;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	UFUNCTION(BlueprintCallable)
	void CloseNote();

	void OpenNote();


	UFUNCTION(BlueprintCallable, Category = "Note")
	static ANoteActor* FindOpenedNoteActor();


	bool IsOpened();

};
