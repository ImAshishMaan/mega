#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WDashCooldown.generated.h"

class UImage;

UCLASS()
class MEGA_API UWDashCooldown : public UUserWidget {
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UImage* BarFill;

	FTimerHandle FillTimerHandle;
	float ElapsedTime;
	float CoolDownTime;

	void UpdateArcAnimateFill();

protected:
	virtual void NativeConstruct() override;
	
};
