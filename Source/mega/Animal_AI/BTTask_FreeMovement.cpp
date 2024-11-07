#include "BTTask_FreeMovement.h"
#include "AIController.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_FreeMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UWorld* World = GetWorld();

	AAIController* Controller = OwnerComp.GetAIOwner();
	if(Controller) {
		APawn* Pawn = Cast<APawn>(Controller->GetPawn());
		if(Pawn) {
			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
			ensureMsgf(NavSys, TEXT("Navigation system not found!"));
			const FVector CurrentActorLocation = Pawn->GetActorLocation();
			const float Radius = FMath::RandRange(MinRadiusDistance, MaxRadiusDistance);
			FNavLocation NextRandLocation;
			const bool FoundLocation = NavSys->GetRandomPointInNavigableRadius(CurrentActorLocation, Radius, NextRandLocation);
			if(FoundLocation) {
				Controller->MoveToLocation(NextRandLocation);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
	
}
