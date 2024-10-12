#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class MEGA_API AProjectile : public AActor {
	GENERATED_BODY()

public:
	AProjectile();
	void ShowImpactParticles(UPrimitiveComponent* HitComponent,  AActor* OtherActor);

	void ResetProjectile();
	void ActivateProjectile();
	bool IsActive() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse, const FHitResult& Hit);
	void PlayImpactSound();

	UPROPERTY(EditAnywhere)
	float Damage = 20.0f;

	// Impact particles for different materials
	UPROPERTY(EditAnywhere)
	UParticleSystem* MetalImpactParticle;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* WoodImpactParticle;

	UPROPERTY(EditAnywhere)
	USoundCue* ImpactSound;

private:
	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	UPROPERTY()
	UParticleSystemComponent* TracerComponent;

	bool bInUse;

public:

	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};
