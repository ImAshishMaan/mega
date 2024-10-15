#pragma once

#include "CoreMinimal.h"
#include "MWorldUserWidget.h"
#include "HealthBarWIdget.generated.h"

class UProgressBar;

UCLASS()
class MEGA_API UHealthBarWIdget : public UMWorldUserWidget {
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateHealth(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage);

};
