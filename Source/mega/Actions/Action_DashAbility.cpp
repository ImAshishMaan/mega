#include "Action_DashAbility.h"
#include "GameFramework/Character.h"

void UAction_DashAbility::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	// Find movement Direction and play animation
	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(Character) {
		if(Character->GetVelocity().X > 0.0f) {
			Character->PlayAnimMontage(DashRightAnim);
		} else if(Character->GetVelocity().X < 0.0f) {
			Character->PlayAnimMontage(DashLeftAnim);
		} else if(Character->GetVelocity().Y > 0.0f) {
			Character->PlayAnimMontage(DashForwardAnim);
		} else if(Character->GetVelocity().Y < 0.0f) {
			Character->PlayAnimMontage(DashBackwardAnim);
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
