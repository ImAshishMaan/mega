#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class AMegaHUD;
class UButton;

UCLASS()
class MEGA_API UPauseMenu : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UFUNCTION()
	void ClosePauseMenu();

	UFUNCTION()
	void OpenSettingMenu();

private:

	UPROPERTY()
	AMegaHUD* MegaHUD;

};
