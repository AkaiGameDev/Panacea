#pragma once

#include "GeometryCollection/GeometryCollectionComponent.h" 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "PotionBottle.generated.h"

class USwitchComponent;
class UInteractiveComponent;
class UMetaSoundSource;
class UAudioComponent;

UCLASS()
class PANACEA_API APotionBottle : public AItem
{
	GENERATED_BODY()
	
public:
	APotionBottle();

	virtual void Interact() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UGeometryCollectionComponent* GeometryCollectionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USwitchComponent* SwitchComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Audio")
	UAudioComponent* MetaSoundAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Audio")
	UMetaSoundSource* MetaSoundSource;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AItem> ActorToSpawn;

	UPROPERTY(EditAnywhere)
	float BreakableDistance;

	UPROPERTY(EditAnywhere)
	bool bHasAmber;

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentFracture(const FChaosBreakEvent& BreakEvent);

	UFUNCTION()
	void OnComponentReleased(UPrimitiveComponent* ReleasedComponent);

	bool bIsBreaked;

	UInteractiveComponent* InteractiveComponent;
};
