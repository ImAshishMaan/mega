#include "MontagesComponent.h"

#include "CombatComponent.h"
#include "mega/Character/MegaCharacter.h"

UMontagesComponent::UMontagesComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}


void UMontagesComponent::BeginPlay() {
	Super::BeginPlay();
}

void UMontagesComponent::PlayFireMontage() {
	if(MegaCharacter == nullptr) return;

	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && FireMontage) {
		// Use the same montage for all weapons using Section
		AnimInstance->Montage_Play(FireMontage);
	}
}

void UMontagesComponent::PlayReloadMontage() {
	if(MegaCharacter == nullptr || CombatComponent->CombatState == ECombatState::ECS_Reloading) return;

	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && ReloadMontage) {
		// Use the same montage for all weapons using Section
		CombatComponent->CombatState = ECombatState::ECS_Reloading;
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUFunction(this, "OnFireMontageEnded");
		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, FireMontage);
		AnimInstance->Montage_Play(ReloadMontage);
	}
}

void UMontagesComponent::OnFireMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	// this is not working
	if(MegaCharacter == nullptr) return;
	CombatComponent->CombatState = ECombatState::ECS_Unoccupied;
	// Update HUD Weapon ammo
}
