#include "MegaPlayerController.h"
#include "Components/ProgressBar.h"
#include "mega/HUD/CharacterOverlayWidget.h"
#include "mega/HUD/MegaHUD.h"

void AMegaPlayerController::BeginPlay() {
	Super::BeginPlay();

	MegaHUD = Cast<AMegaHUD>(GetHUD());
	
}

void AMegaPlayerController::SetHUDHealth(float Health, float MaxHealth) {
	MegaHUD = MegaHUD == nullptr ? Cast<AMegaHUD>(GetHUD()) : MegaHUD;
	bool bHUDValid = MegaHUD && MegaHUD->CharacterOverlay && MegaHUD->CharacterOverlay->HealthBar;

	if(bHUDValid) {
		const float HealthPercent = Health / MaxHealth;
		MegaHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
	}
}

