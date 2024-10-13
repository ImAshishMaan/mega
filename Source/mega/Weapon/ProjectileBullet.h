#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

class UMaterialInterface;

UCLASS()
class MEGA_API AProjectileBullet : public AProjectile {
	GENERATED_BODY()

public:
	AProjectileBullet();

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

private:

	UPROPERTY(EditAnywhere, Category = "Decal")
	UMaterialInterface* DecalMaterial;

	void SpawnBulletHoles(const FHitResult& ImpactResult);

};
