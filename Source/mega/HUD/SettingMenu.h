#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingMenu.generated.h"

class UButton;
class AMegaHUD;

UCLASS()
class MEGA_API USettingMenu : public UUserWidget {
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UFUNCTION()
	void CloseSettingMenu();

private:
	UPROPERTY()
	AMegaHUD* MegaHUD;
	
};
