#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontagesComponent.generated.h"


class UCombatComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEGA_API UMontagesComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UMontagesComponent();
	friend class AMegaCharacter;

	void PlayFireMontage();
	UFUNCTION()
	void OnFireMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void PlayReloadMontage();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AMegaCharacter* MegaCharacter;

	UPROPERTY()
	UCombatComponent* CombatComponent;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	UAnimMontage* ReloadMontage;

};
