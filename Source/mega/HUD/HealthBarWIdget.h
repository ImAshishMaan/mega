#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWIdget.generated.h"

class USizeBox;
class UProgressBar;

UCLASS()
class MEGA_API UHealthBarWIdget : public UUserWidget {
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void NativeConstruct() override;

	UFUNCTION()
	void UpdateHealth(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage);

public:

	// Set it to the HealthBarWidget Class in the BP.
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;

};
