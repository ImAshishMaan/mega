#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_AnimalController.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API AAI_AnimalController : public AAIController {
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviourTree;
	
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

public:
	
	void UpdateHunger(bool IsHungry);
	void UpdateEating(bool IsEating);
	
};
