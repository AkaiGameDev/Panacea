#include "FootstepComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"

UFootstepComponent::UFootstepComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    VelocityThreshold = 10.0f; // Default threshold value
    TimeGap = 0.2f;
}

void UFootstepComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UFootstepComponent::CheckVelocity, TimeGap, true);
    }
}

void UFootstepComponent::CheckVelocity()
{
    if (!OwnerCharacter || !OwnerCharacter->GetCharacterMovement() || OwnerCharacter->GetCharacterMovement()->IsFalling())
    {
        return;
    }



    float CurrentVelocity = OwnerCharacter->GetCharacterMovement()->Velocity.Size();

    if (CurrentVelocity > VelocityThreshold)
    {
        if (FootstepSound)
        {
            UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, OwnerCharacter->GetActorLocation());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FootstepSound is not set in FootstepComponent"));
        }
    }
}
