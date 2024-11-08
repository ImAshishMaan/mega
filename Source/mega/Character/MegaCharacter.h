#pragma once

#include <string>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "CoreMinimal.h"
#include "TimerManager.h"
#include "mega/MegaComponents/CombatComponent.h"
#include "MegaCharacter.generated.h"

USTRUCT()
struct FInteractionData {
	GENERATED_USTRUCT_BODY()

	FInteractionData() : 
	CurrentInteractable(nullptr),
	LastInteractionCheckTime(0.0f)
	{};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

class IInteractionInterface;
class UActionComponent;
class UAbilityComponent;
class UAttributeComponent;
class UCameraComponent;
class USpringArmComponent;
class UMontagesComponent;
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
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ChangePOVAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PrimaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SecondaryWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* QAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TransformButtonAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractionButtonAction;

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
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void ReloadButtonPressed();
	void PrimaryWeaponButtonPressed();
	void SecondaryWeaponButtonPressed();
	void ChangePOVButtonPressed();
	void QAbilityButtonPressed();
	void TransformButtonPressed();
	void InteractionButtonPressed();
	void InteractionButtonReleased();


	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage);

	UPROPERTY()
	AMegaHUD* MegaHUD;
	
	UPROPERTY(VisibleAnywhere, Category = "Character | Interactable")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	float InteractionCheckFrequency = 0.1f;

	float InteractionCheckDistance = 1000.0f;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	// Functions

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* HumanMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraTP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraFP;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMontagesComponent* MontagesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UActionComponent* ActionComponent;

	UPROPERTY()
	AWeapon* OverlappingWeapon;

	/*
	 * Characters Data
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "01 - Characters Meshes")
	TMap<ECharacterForm, USkeletalMesh*> CharacterMeshesMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "01 - Characters Meshes")
	TMap<ECharacterForm, UClass*> CharacterAnimsMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "01 - Characters Meshes")
	TMap<ECharacterForm, UMaterialInterface*> CharacterMaterialMap;

	ECharacterForm CurrentCharacterForm = ECharacterForm::Human;
	
public:
	void SetOverlappingWeapon(AWeapon* Weapon);
	void ChangeForm(ECharacterForm NewCharacterForm);

	bool IsWeaponEquipped();

	FORCEINLINE AWeapon* GetEquippedWeapon() { return CombatComponent->EquippedWeapon; }
};
