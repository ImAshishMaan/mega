#pragma once

#include "CoreMinimal.h"
#include "mega/Enums.h"
#include "UObject/Interface.h"
#include "AnimationInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAnimationInterface : public UInterface {
	GENERATED_BODY()
	
};

/**
 * 
 */
class MEGA_API IAnimationInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animation")
	void UpdateCharacterState(ECharacterState NewState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animation")
	void GroundDistance(float CurrentGroundDistance);

public:
};
