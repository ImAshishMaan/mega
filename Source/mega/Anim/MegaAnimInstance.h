#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "MegaAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API UMegaAnimInstance : public UAnimInstance, public IAnimationInterface {
	GENERATED_BODY()

public:
	UMegaAnimInstance();
	virtual void UpdateCharacterState_Implementation(ECharacterState NewState);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	ECharacterState CharacterState;

};
