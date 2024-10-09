#pragma once

#include "CoreMinimal.h"
#include "WeaponEnums.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;

UCLASS()
class MEGA_API AWeapon : public AActor {
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;

	virtual void Fire(const FVector& HitTarget);
	void Dropped();
	void SetWeaponState(EWeaponState State);
	void OnWeaponStateSet();

	/*
	 * Automatic Fire
	 */
	UPROPERTY(EditAnywhere, Category = "Fire")
	float FireDelay = 0.1f;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UPROPERTY(EditAnywhere)
	int32 MagCapacity = 30;

	void SpendRound();

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	
};
