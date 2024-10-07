#include "CombatComponent.h"

#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "mega/Enums.h"
#include "mega/Character/MegaCharacter.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "mega/Weapon/Weapon.h"

UCombatComponent::UCombatComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}


void UCombatComponent::BeginPlay() {
	Super::BeginPlay();
	if(MegaCharacter)
		MegaMovementComponent = MegaCharacter->GetCharacterMovement();

	// call this from character class with layer in parameters
	SetAnimLayer();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::SetWalkState() {
	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Walking);
		CurrentState = ECharacterState::Walking;
	}
}

void UCombatComponent::SetJogState() {
	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Jogging);
		CurrentState = ECharacterState::Jogging;
	}
}

void UCombatComponent::SetCrouchState() {
	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Crouching);
		CurrentState = ECharacterState::Crouching;
	}
}

void UCombatComponent::SetGroundDistance() {
	UAnimInstance* AnimInstance = MegaCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		FVector start = MegaCharacter->GetActorLocation() - (MegaCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FVector(
			0.f, 0.f, 1.f));
		FVector end = MegaCharacter->GetActorLocation() * FVector(0.f, 0.f, 1000.f);
		FHitResult hit;
		bool OnHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
		if(OnHit) {
			IAnimationInterface::Execute_GroundDistance(AnimInstance, hit.Distance);
		}
	}
}

void UCombatComponent::SetCharacterStates() {
	FCharacterSettings JoggingSettings;
	JoggingSettings.MaxWalkSpeed = 800.f;
	JoggingSettings.MaxAcceleration = 800.f;
	JoggingSettings.BrakingDeceleration = 1200.f;
	JoggingSettings.BrakingFrictionFactor = 1.0f;
	JoggingSettings.BrakingFriction = 0.0f;
	JoggingSettings.bUseSeparateBrakingFriction = true;

	FCharacterSettings WalkingSettings;
	WalkingSettings.MaxWalkSpeed = 250.f;
	WalkingSettings.MaxAcceleration = 250.f;
	WalkingSettings.BrakingDeceleration = 1000.f;
	WalkingSettings.BrakingFrictionFactor = 1.0f;
	WalkingSettings.BrakingFriction = 0.0f;
	WalkingSettings.bUseSeparateBrakingFriction = true;

	FCharacterSettings CrouchingSettings;
	CrouchingSettings.MaxWalkSpeed = 250.f;
	CrouchingSettings.MaxAcceleration = 250.f;
	CrouchingSettings.BrakingDeceleration = 1000.f;
	CrouchingSettings.BrakingFrictionFactor = 1.0f;
	CrouchingSettings.BrakingFriction = 0.0f;
	CrouchingSettings.bUseSeparateBrakingFriction = true;

	StateSettingsMap.Add(ECharacterState::Jogging, JoggingSettings);
	StateSettingsMap.Add(ECharacterState::Walking, WalkingSettings);
	StateSettingsMap.Add(ECharacterState::Crouching, CrouchingSettings);
}

void UCombatComponent::UpdateCharacterStateWithSettings(ECharacterState NewState) {
	CurrentState = NewState;
	FCharacterSettings NewSettings = StateSettingsMap[NewState];

	MegaMovementComponent->MaxWalkSpeed = NewSettings.MaxWalkSpeed;
	MegaMovementComponent->MaxAcceleration = NewSettings.MaxAcceleration;
	MegaMovementComponent->BrakingDecelerationWalking = NewSettings.BrakingDeceleration;
	MegaMovementComponent->BrakingFrictionFactor = NewSettings.BrakingFrictionFactor;
	MegaMovementComponent->BrakingFriction = NewSettings.BrakingFriction;
	MegaMovementComponent->bUseSeparateBrakingFriction = NewSettings.bUseSeparateBrakingFriction;

	if(NewState == ECharacterState::Jogging) {
		SetJogState();
	} else if(NewState == ECharacterState::Walking) {
		SetWalkState();
	} else if(NewState == ECharacterState::Crouching) {
		SetCrouchState();
	}
}


void UCombatComponent::SetAnimLayer() {
	if(CurrentEquipped != LastEquipped) {
		TMap<EEquipped, TSubclassOf<UAnimInstance>>::ValueType instance = AnimInstanceMap[CurrentEquipped];
		MegaCharacter->GetMesh()->LinkAnimClassLayers(instance);
		SetCharacterStates();
		UpdateCharacterStateWithSettings(CurrentState);
		LastEquipped = CurrentEquipped;
	}
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip) {
	if(WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	// Attach to actor;
	const USkeletalMeshSocket* Socket = MegaCharacter->GetMesh()->GetSocketByName("RightHandSocket");
	if(Socket) {
		Socket->AttachActor(EquippedWeapon, MegaCharacter->GetMesh());
	}

	EquippedWeapon->SetOwner(MegaCharacter);
	// call Set hud weapon ammo in weapon class
	// call set Hud carried ammo in player controller class
	


	// Play equip sound from weapon class
}
