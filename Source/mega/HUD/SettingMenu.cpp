#include "SettingMenu.h"
#include "MegaHUD.h"
#include "Components/Button.h"

void USettingMenu::NativeConstruct() {
	Super::NativeConstruct();

	if(CloseButton) {
		CloseButton->OnClicked.AddDynamic(this, &USettingMenu::CloseSettingMenu);
	}
}

void USettingMenu::CloseSettingMenu() {
	MegaHUD = MegaHUD == nullptr ? Cast<AMegaHUD>(GetOwningPlayer()->GetHUD()) : MegaHUD;
	APlayerController* PlayerController = GetOwningPlayer();
	if(MegaHUD) {
		MegaHUD->PauseUIManager->PopWidget(PlayerController);
	}
}
