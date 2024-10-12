#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class AProjectile;
class AMegaCharacter;
class UAnimMontage;
class UParticleSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEGA_API UAbilityComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UAbilityComponent();
	UFUNCTION()
	void AttackDelay_Elapsed(const FVector& HitTarget);
	friend class AMegaCharacter;

	void MagicAttack(const FVector& HitTarget);

protected:
	/*
	 * Ability System Initialization
	 * It's called from MegaCharacter.cpp (It's like a BeginPlay)
	 * Cannot use BeginPlay because combat BeginPlay might be called before character able to initialize some values in AbilityComponent
	 */
	void InitializeAbilitySystem();

private:
	UPROPERTY()
	AMegaCharacter* MegaCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FPrimaryAssetId ProjectileAssetId;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MagicFireDelay;

public:
	FORCEINLINE float GetMagicFireDelay() { return MagicFireDelay; }
};
