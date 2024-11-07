#include "BTTask_FindFood.h"
#include "AIController.h"
#include "AI_AnimalCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTTask_FindFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if(AIController) {
		APawn* Pawn = Cast<APawn>(AIController->GetPawn());
		AAI_AnimalCharacter* AICharacter = Cast<AAI_AnimalCharacter>(Pawn);
		if(Pawn) {
			TArray<AActor*> AllActors;
			UGameplayStatics::GetAllActorsOfClass(AIController, AICharacter->GetFoodActor(), AllActors);

			// last used actor
			if(AllActors.Num() > 0) {
				AActor* ClosestActor = nullptr;

				for(AActor* Actor: AllActors) {
					if(!ClosestActor) {
						ClosestActor = Actor;
					} else if(Actor->GetDistanceTo(Pawn) < ClosestActor->GetDistanceTo(Pawn)) {
						ClosestActor = Actor;
					}
				}
				if(ClosestActor) {
					AIController->MoveToActor(ClosestActor, AccepetanceRadius);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}


	return EBTNodeResult::Failed;
}
