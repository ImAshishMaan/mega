#include "MegaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "mega/Interfaces/InteractionInterface.h"
#include "mega/MegaComponents/ActionComponent.h"
#include "mega/MegaComponents/AttributeComponent.h"
#include "mega/MegaComponents/CombatComponent.h"
#include "mega/MegaComponents/MontagesComponent.h"
#include "mega/PlayerController/MegaPlayerController.h"
#include "mega/Weapon/Weapon.h"
#include "DrawDebugHelpers.h"
#include "mega/HUD/MegaHUD.h"
#include "mega/MegaComponents/InventoryComponent.h"
#include "mega/World/ItemPickups.h"

AMegaCharacter::AMegaCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	HumanMeshComponent = GetMesh();

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 350.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraRotationLag = true;

	FollowCameraTP = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCameraTP"));
	FollowCameraTP->SetupAttachment(SpringArmComponent);
	FollowCameraTP->bUsePawnControlRotation = false;
	FollowCameraTP->SetRelativeLocation(FVector(0.0f, 50.0f, 50.0f));

	FollowCameraFP = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCameraFP"));
	FollowCameraFP->SetupAttachment(GetMesh(), "head");
	FollowCameraFP->bUsePawnControlRotation = true;
	FollowCameraFP->SetAutoActivate(false);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	MontagesComponent = CreateDefaultSubobject<UMontagesComponent>(TEXT("MontagesComponent"));
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComponent"));
	
	PlayerInventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	PlayerInventoryComponent->SetSlotsCapacity(20);
	PlayerInventoryComponent->SetWeightCapacity(50.0f);
	
}


void AMegaCharacter::BeginPlay() {
	Super::BeginPlay();
	AddMappingContext();

	MegaHUD = Cast<AMegaHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void AMegaCharacter::SetOverlappingWeapon(AWeapon* Weapon) {
	if(Weapon) {
		OverlappingWeapon = Weapon;
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
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AMegaCharacter::StartWalking);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AMegaCharacter::StopWalking);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMegaCharacter::Crouch);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMegaCharacter::Equip);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AMegaCharacter::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMegaCharacter::AimButtonReleased);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMegaCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AMegaCharacter::FireButtonReleased);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMegaCharacter::ReloadButtonPressed);
		EnhancedInputComponent->BindAction(PrimaryWeaponAction, ETriggerEvent::Triggered, this, &AMegaCharacter::PrimaryWeaponButtonPressed);
		EnhancedInputComponent->BindAction(SecondaryWeaponAction, ETriggerEvent::Triggered, this, &AMegaCharacter::SecondaryWeaponButtonPressed);
		EnhancedInputComponent->BindAction(ChangePOVAction, ETriggerEvent::Triggered, this, &AMegaCharacter::ChangePOVButtonPressed);
		EnhancedInputComponent->BindAction(QAbilityAction, ETriggerEvent::Triggered, this, &AMegaCharacter::QAbilityButtonPressed);
		EnhancedInputComponent->BindAction(DashAbilityAction, ETriggerEvent::Triggered, this, &AMegaCharacter::DashAbilityButtonPressed);
		
		EnhancedInputComponent->BindAction(InteractionButtonAction, ETriggerEvent::Triggered, this, &AMegaCharacter::InteractionButtonPressed);
		EnhancedInputComponent->BindAction(InteractionButtonAction, ETriggerEvent::Completed, this, &AMegaCharacter::InteractionButtonReleased);
		EnhancedInputComponent->BindAction(ToggleInventoryMenuAction, ETriggerEvent::Triggered, this, &AMegaCharacter::ToggleInventoryMenuPressed);

		EnhancedInputComponent->BindAction(TransformButtonAction, ETriggerEvent::Triggered, this, &AMegaCharacter::TransformButtonPressed);
	}
}

void AMegaCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	if(CombatComponent) {
		CombatComponent->MegaCharacter = this;
		CombatComponent->MontagesComponent = MontagesComponent;
		CombatComponent->InitializeCombatSystem();
	}
	if(MontagesComponent) {
		MontagesComponent->MegaCharacter = this;
		MontagesComponent->CombatComponent = CombatComponent;
		MontagesComponent->InitializeMontagesSystem();
	}

	if(AttributeComponent) {
		AttributeComponent->OnHealthChanged.AddDynamic(this, &AMegaCharacter::OnHealthChanged);
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
	} else {
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
	if(OverlappingWeapon && CombatComponent && CombatComponent->EquippedWeapon == nullptr) {
		if(OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_Rifle && CombatComponent->PrimaryWeapon == nullptr) {
			CombatComponent->SetAnimLayer(EEquipped::Rifle);
			CombatComponent->EquipWeapon(OverlappingWeapon);
		} else if(OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_Rifle && CombatComponent->PrimaryWeapon !=
			nullptr) {
			// TODO: swap weapons
			UE_LOG(LogTemp, Warning, TEXT("Rifle already equipped: TODO : swap weapons"));
		}
		if(OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_Pistol && CombatComponent->SecondaryWeapon ==
			nullptr) {
			CombatComponent->SetAnimLayer(EEquipped::Pistol);
			CombatComponent->EquipWeapon(OverlappingWeapon);
		} else if(OverlappingWeapon->GetWeaponType() == EWeaponType::EWT_Pistol && CombatComponent->SecondaryWeapon !=
			nullptr) {
			// TODO: swap weapons
			UE_LOG(LogTemp, Warning, TEXT("Pistol already equipped: TODO : swap weapons"));
		}

		OverlappingWeapon = nullptr;
	} else if(CombatComponent && CombatComponent->EquippedWeapon) {
		CombatComponent->SetAnimLayer(EEquipped::UnEquipped);
		CombatComponent->DropWeapon();
	}
}

void AMegaCharacter::AimButtonPressed() {
	if(CombatComponent) {
		CombatComponent->UpdateCharacterStateWithSettings(ECharacterState::Walking);
		CombatComponent->SetAimButtonPressed(true);
	}
}

void AMegaCharacter::AimButtonReleased() {
	if(CombatComponent) {
		CombatComponent->UpdateCharacterStateWithSettings(ECharacterState::Jogging);
		CombatComponent->SetAimButtonPressed(false);
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

void AMegaCharacter::PrimaryWeaponButtonPressed() {
	if(CombatComponent) {
		if(CombatComponent->PrimaryWeapon && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponType() == CombatComponent->PrimaryWeapon->GetWeaponType()) {
			CombatComponent->SetAnimLayer(EEquipped::UnEquipped);
			CombatComponent->HolsterWeapon();
		} else if(CombatComponent->PrimaryWeapon) {
			CombatComponent->SetAnimLayer(EEquipped::Rifle);
			CombatComponent->EquipWeapon(CombatComponent->PrimaryWeapon);
		}
	}
}

void AMegaCharacter::SecondaryWeaponButtonPressed() {
	if(CombatComponent) {
		if(CombatComponent->SecondaryWeapon && CombatComponent->EquippedWeapon && CombatComponent->EquippedWeapon->GetWeaponType() == CombatComponent->SecondaryWeapon->GetWeaponType()) {
			CombatComponent->SetAnimLayer(EEquipped::UnEquipped);
			CombatComponent->HolsterWeapon();
		} else if(CombatComponent->SecondaryWeapon) {
			CombatComponent->SetAnimLayer(EEquipped::Pistol);
			CombatComponent->EquipWeapon(CombatComponent->SecondaryWeapon);
		}
	}
}

void AMegaCharacter::ChangePOVButtonPressed() {
	if(FollowCameraTP->IsActive()) {
		FollowCameraFP->SetActive(true);
		FollowCameraTP->SetActive(false);
	} else if(FollowCameraFP->IsActive()) {
		FollowCameraTP->SetActive(true);
		FollowCameraFP->SetActive(false);
	}
}

void AMegaCharacter::QAbilityButtonPressed() {
	if(ActionComponent) {
		ActionComponent->StartActionByName(this, "MagicProjectile");
	}
}

void AMegaCharacter::DashAbilityButtonPressed() {
	if(ActionComponent) {
		ActionComponent->StartActionByName(this, "DashAbility");
	}
}

void AMegaCharacter::TransformButtonPressed() {
	if(CurrentCharacterForm == ECharacterForm::Human) {
		ChangeForm(ECharacterForm::Traveler);
	} else {
		ChangeForm(ECharacterForm::Human);
	}
}

void AMegaCharacter::InteractionButtonPressed() {
	BeginInteract();
}

void AMegaCharacter::InteractionButtonReleased() {
	EndInteract();
}

void AMegaCharacter::ToggleInventoryMenuPressed() {
	MegaHUD->ToggleMenu();
}

void AMegaCharacter::ChangeForm(ECharacterForm NewCharacterForm) {
	if(CharacterMeshesMap.Contains(NewCharacterForm)) {
		CurrentCharacterForm = NewCharacterForm;
		HumanMeshComponent->SetSkeletalMesh(CharacterMeshesMap[NewCharacterForm]);
		HumanMeshComponent->SetAnimInstanceClass(CharacterAnimsMap[NewCharacterForm]);
		HumanMeshComponent->SetMaterial(0, CharacterMaterialMap[NewCharacterForm]);

		// TODO: Need to think about this later.
		if(NewCharacterForm == ECharacterForm::Human) {
			if(CombatComponent) {
				CombatComponent->InitializeCombatSystem();
			}
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("CharacterForm %hhd does not exist"), NewCharacterForm);
	}
}

bool AMegaCharacter::IsWeaponEquipped() {
	return (CombatComponent && CombatComponent->EquippedWeapon);
}


void AMegaCharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage) {
	/*
	 * NOTE: I Can Do this With MegaPlayerController. OR I can also use same Delegate directly in CharacterOverlayHealthBar
	 */
	AMegaPlayerController* MegaPlayerController = Cast<AMegaPlayerController>(GetController());
	if(MegaPlayerController) {
		MegaPlayerController->SetHUDHealth(NewHealth, MaxHealth);
	}
}

void AMegaCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) >= InteractionCheckFrequency) {
		PerformInteractionCheck();
	}
}

void AMegaCharacter::PerformInteractionCheck() {
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport) {
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
	if(bScreenToWorld) {
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * InteractionCheckDistance;

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 0.5f);

		FCollisionQueryParams TracerParams;
		TracerParams.AddIgnoredActor(this);

		FHitResult TraceHitResult;

		if(GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility, TracerParams)) {
			if(TraceHitResult.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass())) {
				if(TraceHitResult.GetActor() != InteractionData.CurrentInteractable) {
					FoundInteractable(TraceHitResult.GetActor());
					return;
				}

				if(TraceHitResult.GetActor() == InteractionData.CurrentInteractable) {
					return;
				}
			}
		}

		NoInteractableFound();
	}
}

void AMegaCharacter::FoundInteractable(AActor* NewInteractable) {
	if(IsInteracting()) {
		EndInteract();
	}

	if(InteractionData.CurrentInteractable) {
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	MegaHUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void AMegaCharacter::NoInteractableFound() {
	if(IsInteracting()) {
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if(InteractionData.CurrentInteractable) {
		if(IsValid(TargetInteractable.GetObject())) {
			TargetInteractable->EndFocus();
		}

		MegaHUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AMegaCharacter::BeginInteract() {
	// Verify that nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if(InteractionData.CurrentInteractable) {
		if(IsValid(TargetInteractable.GetObject())) {
			TargetInteractable->BeginInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f)) {
				Interact();
			} else {
				GetWorldTimerManager().SetTimer(
					TimerHandle_Interaction,
					this,
					&AMegaCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false
				);
			}
		}
	}
}

void AMegaCharacter::EndInteract() {
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if(IsValid(TargetInteractable.GetObject())) {
		TargetInteractable->EndInteract();
	}
	
}
void AMegaCharacter::Interact() {
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if(IsValid(TargetInteractable.GetObject())) {
		TargetInteractable->Interact(this);
	}
}

void AMegaCharacter::UpdateInteractionWidget() const {
	if(IsValid(TargetInteractable.GetObject())) {
		MegaHUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AMegaCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop) {
	if(PlayerInventoryComponent->FindMatchingItem(ItemToDrop)) {
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{GetActorLocation() + GetActorForwardVector() * 100.0f};
		const FTransform SpawnTransform{FTransform{GetActorRotation(), SpawnLocation}};

		const int32 RemovedQuantity = PlayerInventoryComponent->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);
		AItemPickups* Pickup = GetWorld()->SpawnActor<AItemPickups>(AItemPickups::StaticClass(), SpawnTransform, SpawnParameters); // TODO: Why we used static class here think about it again (populate the spawned item with data from ??)
		
		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
		
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No matching item in inventory to drop"));
	}
}

