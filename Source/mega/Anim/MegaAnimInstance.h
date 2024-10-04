#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "mega/Character/MegaCharacter.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "MegaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API UMegaAnimInstance : public UAnimInstance, public IAnimationInterface {
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateCharacterState_Implementation(ECharacterState NewState);

	/*
	 * Velocity Data
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "VelocityData")
	FVector CharacterVelocity;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "VelocityData")
	FVector CharacterVelocity2D;

	void SetVelocityData();

	/*
	 * Character Movement
	 */

	UCharacterMovementComponent* OwnerMovementComponent = nullptr;
	void SetCharacterMovement();

	/*
	 * Character State
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	ECharacterState CurrentState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	ECharacterState IncomingState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	ECharacterState LastFrameState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	bool IsStateChanged;

	void SetCharacterStates();

	/*
	 * Acceleration Data
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AccelerationData")
	FVector Acceleration;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AccelerationData")
	FVector Acceleration2D;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AccelerationData")
	bool IsAccelerating;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AccelerationData")
	FVector PivotAcceleration2D;

	void SetAccelerationData();

	/*
	 * Location Data
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	FRotator WorldRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	FVector WorldLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	float ActorYaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	float LastFrameActorYaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	float DeltaActorYaw;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	float LeanAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	FVector LastFrameWorldLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocationData")
	float DeltaLocation;

	void SetLocationData(float DeltaTime);

	/*
	 * Locomotion Data
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocomotionData")
	float VelocityLocomotionAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocomotionData")
	ELocomotionDirections VelocityLocomotionDirection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocomotionData")
	ELocomotionDirections LastFrameVelocityLocomotionDirection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocomotionData")
	float AccelerationLocomotionAngle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocomotionData")
	ELocomotionDirections AccelerationLocomotionDirection;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LocomotionData")
	float VelocityLocomotionAngleWithOffset;


	void SetLocomotionData();
	ELocomotionDirections CalculateLocomotionDirection(float CurrentLocomotionAngle, float BackwardMin, float BackwardMax,
	                                  float ForwardMin, float ForwardMax, ELocomotionDirections CurrentDirection,
	                                  float DeadZone);

	/*
	 * Root Yaw Offset
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RootYawOffset")
	float RootYawOffset;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RootYawOffset")
	ERootYawOffsetMode RootYawOffsetMode;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RootYawOffset")
	FFloatSpringState RootYawOffsetSpringState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RootYawOffset")
	float TurnYawCurveValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RootYawOffset")
	float LastFrameTurnYawCurveValue;

	void SetRootYawOffset(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void ProcessTurnYawCurve(float DeltaTime);

	/*
	 * Crouch
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Crouch")
	bool IsCrouching;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Crouch")
	bool LastFrameIsCrouching;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Crouch")
	bool CrouchStateChange;

	/*
	 * Jump
	 */

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Jump")
	bool IsJumping;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Jump")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Jump")
	bool IsOnAir;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Jump")
	float TimeToJumpApex;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Jump")
	float GroundDistance;

private:
	AMegaCharacter* OwnerCharacter = nullptr;
};
