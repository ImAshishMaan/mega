#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class MEGA_API ASAIController : public AAIController {
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviourTree;
	
	virtual void BeginPlay() override;
};
