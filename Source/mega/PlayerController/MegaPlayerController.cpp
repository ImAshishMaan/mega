#include "MegaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "mega/Anim/MegaAnimInstance.h"
#include "mega/HUD/CharacterHealthWidget.h"
#include "mega/HUD/MegaHUD.h"
#include "UObject/ConstructorHelpers.h"

AMegaPlayerController::AMegaPlayerController() {
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputContextObj(TEXT("InputMappingContext'/Game/Inputs/IMC_Mega.IMC_Mega'"));
	if(InputContextObj.Succeeded()) {
		DefaultMappingContext = InputContextObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionObj(TEXT("InputAction'/Game/Inputs/Actions/IA_Pause.IA_Pause'"));
	if(InputActionObj.Succeeded()) {
		TogglePauseAction = InputActionObj.Object;
	}
}


void AMegaPlayerController::BeginPlay() {
	Super::BeginPlay();

	MegaHUD = Cast<AMegaHUD>(GetHUD());
}


void AMegaPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem && DefaultMappingContext) {
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if(EnhancedInputComponent) {
		EnhancedInputComponent->BindAction(TogglePauseAction, ETriggerEvent::Triggered, this, &AMegaPlayerController::TogglePausePressed);
	}
}

void AMegaPlayerController::EnableMouse(bool bEnable) {
	bShowMouseCursor = bEnable;
	bEnableClickEvents = bEnable;
	bEnableMouseOverEvents = bEnable;
	// Set the input mode to UI only so that mouse can interact with widgets
	if(bEnable) {
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	} else {
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}

void AMegaPlayerController::TogglePausePressed() {
	UE_LOG(LogTemp, Warning, TEXT("TogglePausePressed"));
	MegaHUD = MegaHUD == nullptr ? Cast<AMegaHUD>(GetHUD()) : MegaHUD;
	if(MegaHUD) {
		MegaHUD->PauseUIManager->ShowWidget(MegaHUD->PauseMenuClass, this);
		EnableMouse(true);
	}
}

void AMegaPlayerController::SetHUDHealth(float Health, float MaxHealth) {
	MegaHUD = MegaHUD == nullptr ? Cast<AMegaHUD>(GetHUD()) : MegaHUD;
	bool bHUDValid = MegaHUD && MegaHUD->CharacterOverlay;

	if(bHUDValid) {
		MegaHUD->CharacterOverlay->SetHealth(Health, MaxHealth);
	}
}
