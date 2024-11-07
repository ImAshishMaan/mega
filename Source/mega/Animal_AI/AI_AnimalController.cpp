#include "AI_AnimalController.h"

#include "AI_AnimalCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

void AAI_AnimalController::BeginPlay() {
	Super::BeginPlay();

	if(BehaviourTree) {
		RunBehaviorTree(BehaviourTree);
	}
}

void AAI_AnimalController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	Super::OnMoveCompleted(RequestID, Result);

	if(Result.IsSuccess()) {
		AAI_AnimalCharacter* AIChar = Cast<AAI_AnimalCharacter>(GetPawn());
		if(AIChar && AIChar->GetIsHungry() && AIChar->GetIsEating() == false) {
			AIChar->StartEatingTimer();
		}
	} else if(Result.IsFailure()) {
		// Handle failure (could not reach the destination)
	} else if(Result.IsInterrupted()) {
		// Handle aborted (movement stopped before reaching the destination)
	}
}

void AAI_AnimalController::UpdateHunger(bool IsHungry) {
	UBlackboardComponent* board = GetBlackboardComponent();
	if(board) {
		board->SetValueAsBool("IsHungry", IsHungry);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Blackboard not found"));
	}
	//GetBlackboardComponent()->SetValueAsBool("IsHungry", IsHungry);
}

void AAI_AnimalController::UpdateEating(bool IsEating) {
	GetBlackboardComponent()->SetValueAsBool("IsEating", IsEating);
}
