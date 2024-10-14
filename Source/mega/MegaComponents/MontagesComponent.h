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
	/*
	 * Montages System Initialization
	 * It's called from MegaCharacter.cpp (It's like a BeginPlay)
	 * Cannot use BeginPlay because montage BeginPlay might be called before character able to initialize some values in MontagesComponent
	 */
	void InitializeMontagesSystem();

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
