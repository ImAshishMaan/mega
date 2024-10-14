#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MagicProjectileBase.h"
#include "MagicProjectile.generated.h"


UCLASS()
class MEGA_API AMagicProjectile : public AMagicProjectileBase {
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	AMagicProjectile();
	
};
