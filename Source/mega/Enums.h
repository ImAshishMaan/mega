#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	Walking,
	Jogging,
	Crouching,
};

UENUM(BlueprintType)
enum class ELocomotionDirections : uint8 {
	Forward,
	Backward,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8 {
	Accumulate,
	BlendOut,
	Hold
};