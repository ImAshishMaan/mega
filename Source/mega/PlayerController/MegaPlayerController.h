#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/InputAction.h"
#include "MegaPlayerController.generated.h"

class AMegaHUD;
class UInputMappingContext;
class UInputAction;

UCLASS()
class MEGA_API AMegaPlayerController : public APlayerController {
	GENERATED_BODY()
	
public:

	AMegaPlayerController();
	void SetHUDHealth(float Health, float MaxHealth);
	virtual void SetupInputComponent() override;

	UPROPERTY()
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY()
	UInputAction* TogglePauseAction;
	void TogglePausePressed();

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY()
	AMegaHUD* MegaHUD;

	
};
