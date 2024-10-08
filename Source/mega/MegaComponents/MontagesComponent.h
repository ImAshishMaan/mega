#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MontagesComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEGA_API UMontagesComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UMontagesComponent();
	friend class AMegaCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void PlayFireMontage();
	void PlayReloadMontage();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AMegaCharacter* MegaCharacter;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Weapons")
	UAnimMontage* ReloadMontage;

};
