#include "MegaCharacter.h"

#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
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
	}
}

void AMegaCharacter::SetJogState() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AnimInstance->Implements<UAnimationInterface>()) {
		IAnimationInterface::Execute_UpdateCharacterState(AnimInstance, ECharacterState::Jogging);
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
