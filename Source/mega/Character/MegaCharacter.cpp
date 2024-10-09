#include "MegaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "mega/MegaComponents/CombatComponent.h"
#include "mega/MegaComponents/MontagesComponent.h"
#include "mega/PlayerController/MegaPlayerController.h"

AMegaCharacter::AMegaCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	MontagesComponent = CreateDefaultSubobject<UMontagesComponent>(TEXT("MontagesComponent"));
	
}


void AMegaCharacter::BeginPlay() {
	Super::BeginPlay();
	AddMappingContext();
	
}

void AMegaCharacter::SetOverlappingWeapon(AWeapon* Weapon) {
	if(Weapon) {
		OverlappingWeapon = Weapon;
	}
}

void AMegaCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMegaCharacter::AddMappingContext() {
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMegaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMegaCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMegaCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMegaCharacter::StartJumping);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMegaCharacter::StopJumping);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AMegaCharacter::StartWalking);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AMegaCharacter::StopWalking);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMegaCharacter::Crouch);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMegaCharacter::Equip);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMegaCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMegaCharacter::FireButtonReleased);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMegaCharacter::ReloadButtonPressed);
	}
}

void AMegaCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if(CombatComponent) {
		CombatComponent->MegaCharacter = this;
		CombatComponent->MegaPlayerController = Cast<AMegaPlayerController>(GetController());
		CombatComponent->MontagesComponent = MontagesComponent;
	}
	if(MontagesComponent) {
		MontagesComponent->MegaCharacter = this;
		MontagesComponent->CombatComponent = CombatComponent;
	}
}

void AMegaCharacter::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMegaCharacter::Look(const FInputActionValue& Value) {
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMegaCharacter::StartWalking() {
	if(CombatComponent) {
		CombatComponent->UpdateCharacterStateWithSettings(ECharacterState::Walking);
	}
}
void AMegaCharacter::StopWalking() {
	if(CombatComponent) {
		CombatComponent->UpdateCharacterStateWithSettings(ECharacterState::Jogging);
	}
}
void AMegaCharacter::Crouch() {
	if(CombatComponent && CombatComponent->CurrentState != ECharacterState::Crouching) {
		CombatComponent->UpdateCharacterStateWithSettings(ECharacterState::Crouching);
	}else {
		CombatComponent->UpdateCharacterStateWithSettings(ECharacterState::Jogging);
	}
}

void AMegaCharacter::StartJumping() {
	Jump();
}

void AMegaCharacter::StopJumping() {
	Super::StopJumping();
}

void AMegaCharacter::Equip() {
	// TODO: to unequip specific weapon better use struct for sockets names 
	if(OverlappingWeapon) {
		if(CombatComponent) {
			CombatComponent->SetAnimLayer(EEquipped::Rifle);
			CombatComponent->EquipWeapon(OverlappingWeapon);
		}
	}
}

void AMegaCharacter::FireButtonPressed() {
	if(CombatComponent) {
		CombatComponent->FireButtonPressed(true);
	}
}
void AMegaCharacter::FireButtonReleased() {
	if(CombatComponent) {
		CombatComponent->FireButtonPressed(false);
	}
}

void AMegaCharacter::ReloadButtonPressed() {
	if(MontagesComponent) {
		MontagesComponent->PlayReloadMontage();
	}
}
