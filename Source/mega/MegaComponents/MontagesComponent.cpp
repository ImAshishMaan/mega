#include "MontagesComponent.h"

#include "mega/Character/MegaCharacter.h"

UMontagesComponent::UMontagesComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}


void UMontagesComponent::BeginPlay() {
	Super::BeginPlay();
}


void UMontagesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
	if(MegaCharacter == nullptr) return;

	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && ReloadMontage) {
		// Use the same montage for all weapons using Section
		AnimInstance->Montage_Play(ReloadMontage);
	}
}
