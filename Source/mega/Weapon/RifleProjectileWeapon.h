#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RifleProjectileWeapon.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class MEGA_API ARifleProjectileWeapon : public AWeapon {
	GENERATED_BODY()

protected:
	virtual void Fire(const FVector& HitTarget) override;

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;
	
};
