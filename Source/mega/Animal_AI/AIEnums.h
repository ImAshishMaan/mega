#pragma once


UENUM(BlueprintType)
enum class AIState : uint8 {
	FreeMovement,
	Eating,
	Hungry,
	Resting,
	Dead
};