#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MagicProjectileBase.h"
#include "MagicProjectile.generated.h"

class UMaterialInterface;

UCLASS()
class MEGA_API AMagicProjectile : public AMagicProjectileBase {
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	void OnActorHIt(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	AMagicProjectile();

	UPROPERTY(EditAnywhere, Category = "Decal")
	UMaterialInterface* DecalMaterial;

	void SpawnDecal(const FHitResult& ImpactResult);
	
};
