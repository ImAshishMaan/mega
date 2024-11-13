#include "Action_DashAbility.h"
#include "GameFramework/Character.h"
#include "mega/Anim/MegaAnimInstance.h"
#include "mega/Character/MegaCharacter.h"

void UAction_DashAbility::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	// Find movement Direction and play animation
	AMegaCharacter* MegaCharacter = Cast<AMegaCharacter>(Instigator);
	if(MegaCharacter) {
		UMegaAnimInstance* AnimInstance = Cast<UMegaAnimInstance>(MegaCharacter->GetMesh()->GetAnimInstance());
		if(AnimInstance) {
			ELocomotionDirections Direction = AnimInstance->AccelerationLocomotionDirection;
			if(Direction == ELocomotionDirections::Forward) {
				MegaCharacter->PlayAnimMontage(DashForwardAnim);
			} else if(Direction == ELocomotionDirections::Backward) {
				MegaCharacter->PlayAnimMontage(DashBackwardAnim);
			} else if(Direction == ELocomotionDirections::Left) {
				MegaCharacter->PlayAnimMontage(DashLeftAnim);
			} else if(Direction == ELocomotionDirections::Right) {
				MegaCharacter->PlayAnimMontage(DashRightAnim);
			}
		}
	}

	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AbilityCoolDown_Elapsed", Instigator);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AbilityCoolDownTime, false);
}

void UAction_DashAbility::AbilityCoolDown_Elapsed(AActor* Instigator) {
	StopAction(Instigator);
}
