#include "SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay() {
	Super::BeginPlay();

	if(ensureMsgf(BehaviourTree, TEXT("BehaviourTree is nullptr! Please assign a behaviour tree in your AI Controller."))) {
		RunBehaviorTree(BehaviourTree);
	}

	/*APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(pawn) {
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", pawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", pawn);
	}*/
}
