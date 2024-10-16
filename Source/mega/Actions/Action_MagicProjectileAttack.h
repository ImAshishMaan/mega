#pragma once

#include "CoreMinimal.h"
#include "MAction.h"
#include "Action_MagicProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API UAction_MagicProjectileAttack : public UMAction {
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSoftClassPtr<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSoftObjectPtr<UParticleSystem> CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

public:

	virtual void StartAction_Implementation(AActor* Instigator) override;
	void HandleProjectileSpawn(ACharacter* InstigatorCharacter);

	UAction_MagicProjectileAttack();
};
