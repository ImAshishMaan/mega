#include "MegaPlayerController.h"
#include "mega/HUD/MegaHUD.h"

void AMegaPlayerController::BeginPlay() {
	Super::BeginPlay();

	MegaHUD = Cast<AMegaHUD>(GetHUD());
	
}
