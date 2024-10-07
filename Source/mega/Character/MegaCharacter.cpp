#include "MegaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "mega/MegaComponents/CombatComponent.h"
#include "mega/PlayerController/MegaPlayerController.h"

AMegaCharacter::AMegaCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	
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
	}
}

void AMegaCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if(CombatComponent) {
		CombatComponent->MegaCharacter = this;
		CombatComponent->MegaPlayerController = Cast<AMegaPlayerController>(GetController());
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
