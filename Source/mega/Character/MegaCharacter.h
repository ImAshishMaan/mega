#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "CoreMinimal.h"
#include "MegaCharacter.generated.h"

class UCombatComponent;
class UInputAction;
class UInputMappingContext;
class AWeapon;

UCLASS()
class MEGA_API AMegaCharacter : public ACharacter {
	GENERATED_BODY()

public:
	AMegaCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To Initialize all Character Components and components variables
	virtual void PostInitializeComponents() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	void AddMappingContext();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartWalking();
	void StopWalking();
	void Crouch();
	void StartJumping();
	virtual void StopJumping() override;
	void Equip();
	

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

private:
	UPROPERTY()
	AWeapon* OverlappingWeapon;

public:
	void SetOverlappingWeapon(AWeapon* Weapon);


	
};
