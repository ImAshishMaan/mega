#include "MegaCharacter.h"

#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
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

	StateSettingsMap.Add(ECharacterState::Jogging, JoggingSettings);
	StateSettingsMap.Add(ECharacterState::Walking, WalkingSettings);
}

void AMegaCharacter::UpdateCharacterStateWithSettings(ECharacterState NewState) {

	FCharacterSettings NewSettings = StateSettingsMap[NewState];

	GetCharacterMovement()->MaxWalkSpeed = NewSettings.MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = NewSettings.MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = NewSettings.BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = NewSettings.BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFriction = NewSettings.BrakingFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = NewSettings.bUseSeparateBrakingFriction;
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
