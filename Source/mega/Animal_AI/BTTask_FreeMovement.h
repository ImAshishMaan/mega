#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FreeMovement.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API UBTTask_FreeMovement : public UBTTaskNode {
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere)
	float MinRadiusDistance = 500.0f;

	UPROPERTY(EditAnywhere)
	float MaxRadiusDistance = 1000.0f;
};
