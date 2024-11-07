#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindFood.generated.h"

class AActor;

UCLASS()
class MEGA_API UBTTask_FindFood : public UBTTaskNode {
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	UPROPERTY(EditAnywhere)
	float AccepetanceRadius = 50.0f;

	
};
