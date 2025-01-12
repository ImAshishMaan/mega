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
	//virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateCharacterState_Implementation(ECharacterState NewState);
	virtual void GroundDistance_Implementation(float CurrentGroundDistance);

	/*
	 * Velocity Data
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VelocityData")
	FVector CharacterVelocity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VelocityData")
	FVector CharacterVelocity2D;

	void SetVelocityData();

	/*
	 * Character Movement
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Component")
	UCharacterMovementComponent* OwnerMovementComponent = nullptr;
	void SetCharacterMovement();

	/*
	 * Character State
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	ECharacterState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	ECharacterState IncomingState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	ECharacterState LastFrameState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	bool IsStateChanged;

	void SetCharacterStates();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	float HoldingTorch = 0.0f;

	/*
	 * Acceleration Data
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AccelerationData")
	FVector Acceleration;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AccelerationData")
	FVector Acceleration2D;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AccelerationData")
	bool IsAccelerating;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AccelerationData")
	FVector PivotAcceleration2D;

	void SetAccelerationData();

	/*
	 * Location Data
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	FRotator WorldRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	FVector WorldLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	float ActorYaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	float LastFrameActorYaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	float DeltaActorYaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	float LeanAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	FVector LastFrameWorldLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocationData")
	float DeltaLocation;

	void SetLocationData(float DeltaTime);

	/*
	 * Locomotion Data
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocomotionData")
	float VelocityLocomotionAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocomotionData")
	ELocomotionDirections VelocityLocomotionDirection = ELocomotionDirections::Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocomotionData")
	ELocomotionDirections LastFrameVelocityLocomotionDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocomotionData")
	float AccelerationLocomotionAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocomotionData")
	ELocomotionDirections AccelerationLocomotionDirection = ELocomotionDirections::Forward;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LocomotionData")
	float VelocityLocomotionAngleWithOffset;


	void SetLocomotionData();
	ELocomotionDirections CalculateLocomotionDirection(float CurrentLocomotionAngle, float BackwardMin, float BackwardMax,
	                                  float ForwardMin, float ForwardMax, ELocomotionDirections CurrentDirection,
	                                  float DeadZone);

	/*
	 * Root Yaw Offset
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RootYawOffset")
	float RootYawOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RootYawOffset")
	ERootYawOffsetMode RootYawOffsetMode = ERootYawOffsetMode::Accumulate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RootYawOffset")
	FFloatSpringState RootYawOffsetSpringState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RootYawOffset")
	float TurnYawCurveValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RootYawOffset")
	float LastFrameTurnYawCurveValue;

	void SetRootYawOffset(float DeltaTime);

	//UFUNCTION(BlueprintCallable)
	void ProcessTurnYawCurve(float DeltaTime);

	/*
	 * Crouch
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crouch")
	bool IsCrouching;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crouch")
	bool LastFrameIsCrouching;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Crouch")
	bool CrouchStateChange;

	/*
	 * Jump
	 */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Jump")
	bool IsJumping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Jump")
	bool IsFalling;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Jump")
	bool IsOnAir;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Jump")
	float TimeToJumpApex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Jump")
	float GroundDistance;

	/*
	 * Neet to implement ground distance
	 */

private:
	AMegaCharacter* OwnerCharacter = nullptr;
};
