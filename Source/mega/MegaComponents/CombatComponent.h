#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "CombatComponent.generated.h"


class AWeapon;
class UCharacterMovementComponent;
class AMegaHUD;
class AMegaPlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEGA_API UCombatComponent : public UActorComponent , public IAnimationInterface {
	GENERATED_BODY()

public:
	UCombatComponent();
	friend class AMegaCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void SetGroundDistance();
	/*
	 * Character States
	 */

	void SetWalkState();
	void SetJogState();
	void SetCrouchState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState")
	ECharacterState CurrentState = ECharacterState::Jogging;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State Settings")
	TMap<ECharacterState, FCharacterSettings> StateSettingsMap;

	UFUNCTION(BlueprintCallable)
	void SetCharacterStates();

	UFUNCTION(BlueprintCallable)
	void UpdateCharacterStateWithSettings(ECharacterState NewState);

	/*
	 * Animation Layers Data 
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimLayers")
	EEquipped LastEquipped = EEquipped::MaxDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimLayers")
	TMap<EEquipped, TSubclassOf<UAnimInstance>> AnimInstanceMap;

	void SetAnimLayer(EEquipped CurrentEquipped);

	/*
	 * Equip Weapon
	 */
	void EquipWeapon(AWeapon* WeaponToEquip);

protected:
	virtual void BeginPlay() override;

private:
	// Setting MegaCharacter, MegaPlayerController and MegaHUD in MegaCharacter.cpp
	UPROPERTY()
	AMegaCharacter* MegaCharacter;

	UPROPERTY()
	UCharacterMovementComponent* MegaMovementComponent;

	UPROPERTY()
	AMegaPlayerController* MegaPlayerController;

	/*UPROPERTY()
	AMegaHUD* MegaHUD;*/

	UPROPERTY()
	AWeapon* EquippedWeapon;

	
};
