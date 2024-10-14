#include "CombatComponent.h"
#include "MontagesComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "mega/Enums.h"
#include "mega/Character/MegaCharacter.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "mega/Weapon/Weapon.h"

UCombatComponent::UCombatComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::InitializeCombatSystem() {
	if(MegaCharacter) MegaMovementComponent = MegaCharacter->GetCharacterMovement();
	
	ensureMsgf(MegaMovementComponent, TEXT("No CharacterMovementComponent on %s"), *MegaCharacter->GetName());
	
	SetCharacterStates();
	UpdateCharacterStateWithSettings(CurrentState);
	SetAnimLayer(EEquipped::UnEquipped);
}


void UCombatComponent::StartTrace() {
	if(MegaCharacter && EquippedWeapon != nullptr) {
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
	}
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Not a good idea to tick every frame 
	StartTrace();
	
	if(MegaMovementComponent->MovementMode > EMovementMode::MOVE_Walking) {
		SetGroundDistance();
	}
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
		FVector start = MegaCharacter->GetActorLocation() - (MegaCharacter->GetCapsuleComponent()->
		                                                                    GetScaledCapsuleHalfHeight() * FVector(
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
	StateSettingsMap.Add(ECharacterState::Jogging, FCharacterSettings(800.f, 800.f, 1200.f, 1.0f, 0.0f, true));
	StateSettingsMap.Add(ECharacterState::Walking, FCharacterSettings(250.f, 250.f, 1000.f, 1.0f, 0.0f, true));
	StateSettingsMap.Add(ECharacterState::Crouching, FCharacterSettings(250.f, 250.f, 1000.f, 1.0f, 0.0f, true));
}

void UCombatComponent::UpdateCharacterStateWithSettings(ECharacterState NewState) {
	CurrentState = NewState;
	if(StateSettingsMap.Find(NewState)) {
		FCharacterSettings NewSettings = StateSettingsMap[NewState];
		MegaMovementComponent->MaxWalkSpeed = NewSettings.MaxWalkSpeed;
		MegaMovementComponent->MaxAcceleration = NewSettings.MaxAcceleration;
		MegaMovementComponent->BrakingDecelerationWalking = NewSettings.BrakingDeceleration;
		MegaMovementComponent->BrakingFrictionFactor = NewSettings.BrakingFrictionFactor;
		MegaMovementComponent->BrakingFriction = NewSettings.BrakingFriction;
		MegaMovementComponent->bUseSeparateBrakingFriction = NewSettings.bUseSeparateBrakingFriction;
	}

	if(NewState == ECharacterState::Jogging) {
		SetJogState();
	} else if(NewState == ECharacterState::Walking) {
		SetWalkState();
	} else if(NewState == ECharacterState::Crouching) {
		SetCrouchState();
	}
}


void UCombatComponent::SetAnimLayer(EEquipped CurrentEquipped) {
	if(CurrentEquipped != LastEquipped) {
		TMap<EEquipped, TSubclassOf<UAnimInstance>>::ValueType instance = AnimInstanceMap[CurrentEquipped];
		MegaCharacter->GetMesh()->LinkAnimClassLayers(instance);
		LastEquipped = CurrentEquipped;
	}
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip) {
	if(WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;

	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	// Attach to actor;
	AttachActorToComponent("WeaponEquipped");

	EquippedWeapon->SetOwner(MegaCharacter);

	if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Rifle) {
		PrimaryWeapon = EquippedWeapon;
	} else if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol) {
		SecondaryWeapon = EquippedWeapon;
	}
	// call Set hud weapon ammo in weapon class
	// call set Hud carried ammo in player controller class


	// Play equip sound from weapon class
}

void UCombatComponent::DropWeapon() {
	if(EquippedWeapon) {
		if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Rifle) {
			PrimaryWeapon = nullptr;
		}else if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol) {
			SecondaryWeapon = nullptr;
		}
		EquippedWeapon->Dropped();
		EquippedWeapon = nullptr;
	}
}

void UCombatComponent::AttachActorToComponent(FName SocketName) {
	const USkeletalMeshSocket* Socket = MegaCharacter->GetMesh()->GetSocketByName(SocketName);
	if(Socket) {
		Socket->AttachActor(EquippedWeapon, MegaCharacter->GetMesh());
	}
}

void UCombatComponent::HolsterWeapon() {
	if(EquippedWeapon) {
		if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Rifle) {
			PrimaryWeapon = EquippedWeapon;
			AttachActorToComponent("HolsterRifle");
		}else if(EquippedWeapon->GetWeaponType() == EWeaponType::EWT_Pistol) {
			SecondaryWeapon = EquippedWeapon;
			AttachActorToComponent("HolsterPistol");
		}
		UpdateCharacterStateWithSettings(ECharacterState::Jogging);
		EquippedWeapon = nullptr;
	}
}

void UCombatComponent::FireButtonPressed(bool bPressed) {
	bFireButtonPressed = bPressed;
	if(bFireButtonPressed) {
		Fire();
	}
}


bool UCombatComponent::CanFire() {
	if(EquippedWeapon == nullptr) return false;
	return bCanFire && CombatState != ECombatState::ECS_Reloading && bAimButtonPressed;
}

void UCombatComponent::Fire() {
	if(CanFire()) {
		bCanFire = false;
		EquippedWeapon->Fire(HitTarget);
		StartFireTimer();
		MontagesComponent->PlayFireMontage();
	}
}

void UCombatComponent::StartFireTimer() {
	GetWorld()->GetTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay
	);
}

void UCombatComponent::FireTimerFinished() {
	bCanFire = true;
	if(bFireButtonPressed) {
		Fire();
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult) {
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get the crosshair location in the middle of the screen
	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Deproject the crosshair screen location to world space
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld) {
		// Start trace from the crosshair world position or character's location
		FVector Start = CrosshairWorldPosition;

		// The trace end point far ahead in the direction of the crosshair
		FVector End = Start + CrosshairWorldDirection * 80000.0f;

		// Ignore the character for the trace
		FCollisionQueryParams TracerParams;
		TracerParams.AddIgnoredActor(MegaCharacter);

		// Perform the trace
		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			TracerParams
		);

		// If no hit was found, set the trace hit result's ImpactPoint to the end point
		if (!TraceHitResult.bBlockingHit) {
			TraceHitResult.ImpactPoint = End;  // This ensures the bullet goes in the crosshair direction
		}
	}
}
