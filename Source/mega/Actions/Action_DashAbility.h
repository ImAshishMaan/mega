#pragma once

#include "CoreMinimal.h"
#include "MAction.h"
#include "Action_DashAbility.generated.h"


class UWDashCooldown;

UCLASS()
class MEGA_API UAction_DashAbility : public UMAction {
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	UAnimMontage* DashForwardAnim;

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	UAnimMontage* DashBackwardAnim;

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	UAnimMontage* DashLeftAnim;

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	UAnimMontage* DashRightAnim;

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	TSubclassOf<UUserWidget> DashCooldownClass;

	UPROPERTY()
	UWDashCooldown* DashCooldownWidget;

	UPROPERTY(EditAnywhere, Category = "Dash Ability")
	float AbilityCoolDownTime;

	UFUNCTION()
	void AbilityCoolDown_Elapsed(AActor* Instigator);

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	
};
