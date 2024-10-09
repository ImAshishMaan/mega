#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "mega/CombatState.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "CombatComponent.generated.h"


class AWeapon;
class UCharacterMovementComponent;
class AMegaHUD;
class AMegaPlayerController;
class UMontagesComponent;

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
	 * Equip, Fire and drop Weapon
	 */
	void EquipWeapon(AWeapon* WeaponToEquip);
	void DropWeapon();
	void FireButtonPressed(bool bPressed);
	void Fire();

	/*
	 * Trace Under Crosshairs
	 */
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	/*
	 * Combat State
	 */
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	

protected:
	virtual void BeginPlay() override;
	void StartTrace();

private:
	// Setting MegaCharacter, MegaPlayerController and MegaHUD in MegaCharacter.cpp
	UPROPERTY()
	AMegaCharacter* MegaCharacter;

	UPROPERTY()
	UMontagesComponent* MontagesComponent;

	UPROPERTY()
	UCharacterMovementComponent* MegaMovementComponent;

	UPROPERTY()
	AMegaPlayerController* MegaPlayerController;

	/*UPROPERTY()
	AMegaHUD* MegaHUD;*/

	UPROPERTY()
	AWeapon* EquippedWeapon;

	bool bFireButtonPressed;
	bool bAimButtonPressed;

	/*
	 * Automatic Fire
	 */
	FTimerHandle FireTimer;
	bool bCanFire = true;
	void StartFireTimer();
	void FireTimerFinished();
	bool CanFire();

	// Target that are under crosshair live
	FVector HitTarget;

public:
	FORCEINLINE void SetAimButtonPressed(bool bPressed) { bAimButtonPressed = bPressed; }

	
};
