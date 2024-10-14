#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileBullet.generated.h"

class UMaterialInterface;
class UFieldSystem;

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

	UPROPERTY(EditAnywhere, Category = "Field")
	TSubclassOf<AActor> FieldSystem;
	
	void SpawnBulletHoles(const FHitResult& ImpactResult);

};
