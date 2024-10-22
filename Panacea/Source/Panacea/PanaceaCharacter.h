// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InteractionHintWidget.h"
#include "PanaceaCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UMouseDragObjectsComponent;
class UGrabbingSystemComponent;
class UPhysicsHandleComponent;
class UInteractiveComponent;
class UAudioComponent;
class UMetaSoundSource;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APanaceaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Component for Draging objects by mouse */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	UMouseDragObjectsComponent* MouseDragObjectsComponent;

	/** Component for physic handling*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	UPhysicsHandleComponent* PhysicsHandleComponent;

	/** Component for handling interaction */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	UInteractiveComponent* InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Audio")
	UAudioComponent* MetaSoundAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Audio")
	UMetaSoundSource* MetaSoundSource;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Restart Input Action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RestartAction;

	/** Pause Input Action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* PauseAction;

	/** Interact Input Action */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;
	
public:
	APanaceaCharacter();

protected:
	virtual void BeginPlay() override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called When Player Restarts Game */
	void OnRestart();

	/** Called When Player pauses Game */
	void Pause();

	void Interact(const FInputActionValue& Value);
	void JumpFunction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HintInteractionWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;
	
	UPROPERTY()
	UInteractionHintWidget* HintInteractionWidget;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P;}

	void TriggerSoundPlay(const FName& TriggerParameterName);

	/** Returns FirstPersonCameraComponent subobject **/
	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
	/** Returns DefaultMappingContext subobject **/
	UInputMappingContext* GetDefaultMappingContext() const { return DefaultMappingContext; }
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetCrosshairWidget() const;
	
	UFUNCTION(BlueprintCallable)
	UInteractionHintWidget* GetHintInteractionWidget() const;

	UFUNCTION(BlueprintCallable)
	void ShowWidgets() const;
};
