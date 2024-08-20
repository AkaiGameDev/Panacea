#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "NoteWidget.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "PanaceaCharacter.h"
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

	UPROPERTY(EditAnywhere, Category = "UI")
	UTexture2D* NoteImage;

	UPROPERTY()
	UNoteWidget* NoteWidget = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY()
	APanaceaCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOpened = false;

	UPROPERTY()
	UInputMappingContext* CharacterDefaultMappingContext;

public:

	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	UFUNCTION(BlueprintCallable)
	void CloseNote();

	UFUNCTION()
	FString ReplaceLineBreakPlaceholder(const FString& OriginalText);

	void OpenNote();
};
