#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MegaPlayerController.generated.h"

class AMegaHUD;

UCLASS()
class MEGA_API AMegaPlayerController : public APlayerController {
	GENERATED_BODY()
	
public:
	void SetHUDHealth(float Health, float MaxHealth);

protected:
	virtual void BeginPlay() override; 

private:
	UPROPERTY()
	AMegaHUD* MegaHUD;

	
};
