#include "MegaAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UMegaAnimInstance::UpdateCharacterState_Implementation(ECharacterState NewState) {
	IncomingState = NewState;
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
	IsOnAir = OwnerMovementComponent->MovementMode == EMovementMode::MOVE_Flying ? true : false;
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
	
	PivotAcceleration2D = Acceleration2D;
}

void UMegaAnimInstance::SetLocationData(float DeltaTime) {
	LastFrameWorldLocation = WorldLocation;
	WorldLocation = GetOwningActor()->GetActorLocation();
	DeltaLocation = (WorldLocation - LastFrameWorldLocation).Length();

	WorldRotation = GetOwningActor()->GetActorRotation();
	LastFrameActorYaw = ActorYaw;
	ActorYaw = WorldRotation.Yaw;
	DeltaActorYaw = ActorYaw - LastFrameActorYaw;

	
	// NOTE: This might not work
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
	switch(CurrentDirection) {
		case ELocomotionDirections::Forward: {
			bool InForwardRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, (ForwardMin - DeadZone), (ForwardMax + DeadZone));
			if(InForwardRange) {
				return ELocomotionDirections::Forward;
			}
			break;
		}

		case ELocomotionDirections::Backward: {
			if(CurrentLocomotionAngle < (BackwardMin - DeadZone) || CurrentLocomotionAngle > (BackwardMax - DeadZone)) {
				return ELocomotionDirections::Backward;
			}
			break;
		}

		case ELocomotionDirections::Right: {
			bool InRightRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, (ForwardMin - DeadZone), (BackwardMax + DeadZone));
			if(InRightRange) {
				return ELocomotionDirections::Right;
			}
			break;
		}

		case ELocomotionDirections::Left: {
			bool InLeftRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, (BackwardMin - DeadZone), (ForwardMax + DeadZone));
			if(InLeftRange) {
				return ELocomotionDirections::Left;
			}
			break;
		}
	}

	if(CurrentLocomotionAngle < BackwardMin || CurrentLocomotionAngle > BackwardMax) {
		return ELocomotionDirections::Backward;
	} else {
		bool InRange = UKismetMathLibrary::InRange_FloatFloat(CurrentLocomotionAngle, ForwardMin, ForwardMax);
		if(InRange) {
			return ELocomotionDirections::Forward;
		}else {
			if(CurrentLocomotionAngle > 0.f) {
				return ELocomotionDirections::Right;
			}else {
				return ELocomotionDirections::Left;
			}
		}
	}
}

void UMegaAnimInstance::SetRootYawOffset(float DeltaTime) {
	if(RootYawOffsetMode == ERootYawOffsetMode::Accumulate) {
		float Target = RootYawOffset + (DeltaActorYaw * -1);
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(Target);
	}

	if(RootYawOffsetMode == ERootYawOffsetMode::BlendOut) {
		float Target = UKismetMathLibrary::FloatSpringInterp(RootYawOffset, 0.f, RootYawOffsetSpringState, 80.f, 1.f,
		                                                     DeltaTime);
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(Target);
	}

	RootYawOffsetMode = ERootYawOffsetMode::BlendOut;
}

// call from BP
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
