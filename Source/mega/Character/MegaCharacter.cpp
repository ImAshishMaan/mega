#include "MegaCharacter.h"
#include "mega/Interfaces/AnimationInterface.h"

AMegaCharacter::AMegaCharacter() {
	PrimaryActorTick.bCanEverTick = true;
}


void AMegaCharacter::BeginPlay() {
	Super::BeginPlay();
}

void AMegaCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMegaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AMegaCharacter::SetWalkState);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMegaCharacter::SetJogState);
}

void AMegaCharacter::SetWalkState() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->Implements<UAnimationInterface>())
	{
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Walking);
	}
}
void AMegaCharacter::SetJogState() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->Implements<UAnimationInterface>())
	{
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Jogging);
	}
}
