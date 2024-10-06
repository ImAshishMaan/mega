#include "MegaCharacter.h"

#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "mega/Interfaces/AnimationInterface.h"

AMegaCharacter::AMegaCharacter() {
	PrimaryActorTick.bCanEverTick = true;
}


void AMegaCharacter::BeginPlay() {
	Super::BeginPlay();
	AddMappingContext();
	
}

void AMegaCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	SetAnimLayer();
}

void AMegaCharacter::SetAnimLayer() {
	if(CurrentEquipped != LastEquipped) {
		TMap<EEquipped, TSubclassOf<UAnimInstance>>::ValueType instance = AnimInstanceMap[CurrentEquipped];
		GetMesh()->LinkAnimClassLayers(instance);
		SetCharacterStates();
		UpdateCharacterStateWithSettings(CurrentState);
		LastEquipped = CurrentEquipped;
	}
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
	}
}

void AMegaCharacter::SetWalkState() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Walking);
		CurrentState = ECharacterState::Walking;
	}
}

void AMegaCharacter::SetJogState() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Jogging);
		CurrentState = ECharacterState::Jogging;
	}
}

void AMegaCharacter::SetCrouchState() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Crouching);
		CurrentState = ECharacterState::Crouching;
	}
}

void AMegaCharacter::SetGroundDistance() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		FVector start = GetActorLocation() - (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * FVector(
			0.f, 0.f, 1.f));
		FVector end = GetActorLocation() * FVector(0.f, 0.f, 1000.f);
		FHitResult hit;
		bool OnHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECollisionChannel::ECC_Visibility);
		if(OnHit) {
			IAnimationInterface::Execute_GroundDistance(AnimInstance, hit.Distance);
		}
	}
}



void AMegaCharacter::SetCharacterStates() {
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

void AMegaCharacter::UpdateCharacterStateWithSettings(ECharacterState NewState) {
	FCharacterSettings NewSettings = StateSettingsMap[NewState];

	GetCharacterMovement()->MaxWalkSpeed = NewSettings.MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = NewSettings.MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = NewSettings.BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = NewSettings.BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFriction = NewSettings.BrakingFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = NewSettings.bUseSeparateBrakingFriction;

	if(NewState == ECharacterState::Jogging) {
		SetJogState();
	} else if(NewState == ECharacterState::Walking) {
		SetWalkState();
	} else if(NewState == ECharacterState::Crouching) {
		SetCrouchState();
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

void AMegaCharacter::StartJumping() {
	Jump();
}

void AMegaCharacter::StopJumping() {
	Super::StopJumping();
}
