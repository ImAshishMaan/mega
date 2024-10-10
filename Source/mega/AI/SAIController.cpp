#include "SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay() {
	Super::BeginPlay();
	
	RunBehaviorTree(BehaviourTree);

	APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(pawn) {
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", pawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", pawn);
	}
}
