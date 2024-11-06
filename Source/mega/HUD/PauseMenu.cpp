#include "PauseMenu.h"
#include "MegaHUD.h"
#include "Components/Button.h"

void UPauseMenu::NativeConstruct() {
	Super::NativeConstruct();

	if(CloseButton) {
		CloseButton->OnClicked.AddDynamic(this, &UPauseMenu::ClosePauseMenu);
	}

	if(SettingButton) {
		SettingButton->OnClicked.AddDynamic(this, &UPauseMenu::OpenSettingMenu);
	}
}

void UPauseMenu::ClosePauseMenu() {
	MegaHUD = MegaHUD == nullptr ? Cast<AMegaHUD>(GetOwningPlayer()->GetHUD()) : MegaHUD;
	APlayerController* PlayerController = GetOwningPlayer();
	if(MegaHUD) {
		MegaHUD->PauseUIManager->PopWidget(PlayerController);
	}
}

void UPauseMenu::OpenSettingMenu() {
	MegaHUD = MegaHUD == nullptr ? Cast<AMegaHUD>(GetOwningPlayer()->GetHUD()) : MegaHUD;
	APlayerController* PlayerController = GetOwningPlayer();
	if(MegaHUD) {
		MegaHUD->PauseUIManager->ShowWidget(MegaHUD->SettingMenuClass, PlayerController);
	}
}
