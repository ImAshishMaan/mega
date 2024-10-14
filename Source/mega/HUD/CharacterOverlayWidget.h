#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlayWidget.generated.h"

class UProgressBar;

UCLASS()
class MEGA_API UCharacterOverlayWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	
};
