#include "MegaAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UMegaAnimInstance::UpdateCharacterState_Implementation(ECharacterState NewState) {
	IncomingState = NewState;
}

void UMegaAnimInstance::GroundDistance_Implementation(float CurrentGroundDistance) {
	GroundDistance = CurrentGroundDistance;
}

void UMegaAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(OwnerCharacter == nullptr) {
		OwnerCharacter = Cast<AMegaCharacter>(TryGetPawnOwner());
	}

	if(!OwnerCharacter) {
		return;
	}

	if(OwnerMovementComponent == nullptr) {
		OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
	}

	SetCharacterMovement();
	SetVelocityData();
	SetAccelerationData();
	SetLocationData(DeltaSeconds);
	SetLocomotionData();
	SetCharacterStates();
	SetRootYawOffset(DeltaSeconds);
}

void UMegaAnimInstance::SetVelocityData() {
	CharacterVelocity = OwnerMovementComponent->Velocity;
	CharacterVelocity2D = FVector(CharacterVelocity.X, CharacterVelocity.Y, 0.0f);
}

void UMegaAnimInstance::SetCharacterMovement() {
	
}

void UMegaAnimInstance::SetCharacterStates() {
	//State
	LastFrameState = CurrentState;
	CurrentState = IncomingState;
	IsStateChanged = CurrentState != LastFrameState;

	// Crouch
	LastFrameIsCrouching = IsCrouching;
	IsCrouching = IncomingState == ECharacterState::Crouching ? true : false;
	CrouchStateChange = IsCrouching != LastFrameIsCrouching;

	//Jump
	IsOnAir = OwnerMovementComponent->MovementMode == EMovementMode::MOVE_Falling ? true : false;
	IsJumping = IsOnAir && CharacterVelocity.Z > 0;
	IsFalling = IsOnAir && CharacterVelocity.Z < 0;
	if(IsJumping) {
		float VelocityZ = OwnerMovementComponent->GetGravityZ() * OwnerCharacter->GetCharacterMovement()->GravityScale;
		TimeToJumpApex = CharacterVelocity.Z / VelocityZ;
	} else {
		TimeToJumpApex = 0.0f;
	}
}

void UMegaAnimInstance::SetAccelerationData() {
	Acceleration = OwnerMovementComponent->GetCurrentAcceleration();
	Acceleration2D = FVector(Acceleration.X, Acceleration.Y, 0.0f);
	IsAccelerating = !(FMath::IsNearlyZero(Acceleration2D.Size()));
	
	//PivotAcceleration2D is set in Pivot state when pivot state become active 
}

void UMegaAnimInstance::SetLocationData(float DeltaTime) {
	LastFrameWorldLocation = WorldLocation;
	WorldLocation = GetOwningActor()->GetActorLocation();
	DeltaLocation = (WorldLocation - LastFrameWorldLocation).Length();

	WorldRotation = GetOwningActor()->GetActorRotation();
	LastFrameActorYaw = ActorYaw;
	ActorYaw = WorldRotation.Yaw;
	DeltaActorYaw = ActorYaw - LastFrameActorYaw;

	
	/*
	 *  Lean Angle
	 */
	const float Target = DeltaActorYaw / DeltaTime;
	const float Interp = FMath::FInterpTo(LeanAngle, Target, DeltaTime, 6.f);
	LeanAngle = FMath::Clamp(Interp, -90.f, 90.f);
	
}

void UMegaAnimInstance::SetLocomotionData() {
	LastFrameVelocityLocomotionDirection = VelocityLocomotionDirection;
	VelocityLocomotionAngle = CalculateDirection(CharacterVelocity2D, WorldRotation);
	VelocityLocomotionAngleWithOffset = UKismetMathLibrary::NormalizeAxis(VelocityLocomotionAngle - RootYawOffset);

	AccelerationLocomotionAngle = CalculateDirection(Acceleration2D, WorldRotation);

	/*
	 * Calculate Locomotion Direction 
	 */
	AccelerationLocomotionDirection = CalculateLocomotionDirection(AccelerationLocomotionAngle, -130, 130, -50, 50, AccelerationLocomotionDirection, 20);
	VelocityLocomotionDirection = CalculateLocomotionDirection(VelocityLocomotionAngle, -130, 130, -50, 50, VelocityLocomotionDirection, 20);
}

ELocomotionDirections UMegaAnimInstance::CalculateLocomotionDirection(float CurrentLocomotionAngle, float BackwardMin,
	float BackwardMax, float ForwardMin, float ForwardMax, ELocomotionDirections CurrentDirection, float DeadZone) {

	ELocomotionDirections Direction;
	switch(CurrentDirection) {
		case ELocomotionDirections::Forward: {
			bool InForwardRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, (ForwardMin - DeadZone), (ForwardMax + DeadZone));
			if(InForwardRange) {
				return ELocomotionDirections::Forward;
			}
		}

		case ELocomotionDirections::Backward: {
			if(CurrentLocomotionAngle < (BackwardMin - DeadZone) || CurrentLocomotionAngle > (BackwardMax - DeadZone)) {
				return ELocomotionDirections::Backward;
			}
		}

		case ELocomotionDirections::Right: {
			bool InRightRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, (ForwardMax - DeadZone), (BackwardMax + DeadZone));
			if(InRightRange) {
				return ELocomotionDirections::Right;
			}
		}

		case ELocomotionDirections::Left: {
			bool InLeftRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, (BackwardMin - DeadZone), (ForwardMin + DeadZone));
			if(InLeftRange) {
				return ELocomotionDirections::Left;
			}
		}
	}

	if(CurrentLocomotionAngle < BackwardMin || CurrentLocomotionAngle > BackwardMax) {
		Direction = ELocomotionDirections::Backward;
	} else {
		bool InRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, ForwardMin, ForwardMax);
		if(InRange) {
			Direction = ELocomotionDirections::Forward;
		}else {
			if(CurrentLocomotionAngle > 0.f) {
				Direction = ELocomotionDirections::Right;
			}else {
				Direction = ELocomotionDirections::Left;
			}
		}
	}
	return Direction;
}

void UMegaAnimInstance::SetRootYawOffset(float DeltaTime) {
	if(RootYawOffsetMode == ERootYawOffsetMode::Accumulate) {
		float Target = RootYawOffset - DeltaActorYaw;
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(Target);
	}

	if(RootYawOffsetMode == ERootYawOffsetMode::BlendOut) {
		// Lerp to zero if mode is blend out
		float Target = UKismetMathLibrary::FloatSpringInterp(RootYawOffset, 0.f, RootYawOffsetSpringState, 80.f, 1.f,
		                                                     DeltaTime);
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(Target);
	}

	RootYawOffsetMode = ERootYawOffsetMode::BlendOut;
}



// TODO: Implemented same in bp need to figure out how to use this instead of be function
void UMegaAnimInstance::ProcessTurnYawCurve(float DeltaTime) {
	RootYawOffsetMode = ERootYawOffsetMode::Accumulate;
	LastFrameTurnYawCurveValue = TurnYawCurveValue;

	if(GetCurveValue("IsTurning") < 1.f) {
		TurnYawCurveValue = 0.f;
		LastFrameTurnYawCurveValue = 0.f;
	} else {
		TurnYawCurveValue =
			UKismetMathLibrary::SafeDivide(GetCurveValue("root_rotation_Z"), GetCurveValue("IsTurning"));
		if(LastFrameTurnYawCurveValue != 0) {
			float Target = TurnYawCurveValue - LastFrameTurnYawCurveValue;
			float Target2 = RootYawOffset - Target;
			RootYawOffset = UKismetMathLibrary::NormalizeAxis(Target2);
		}
	}
}
