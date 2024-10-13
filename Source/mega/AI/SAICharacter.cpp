#include "SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "mega/MegaComponents/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter() {
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));
	
}

void ASAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* SeenPawn) {
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController) {
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

		BlackboardComp->SetValueAsObject("TargetActor", SeenPawn);

		DrawDebugString(GetWorld(), SeenPawn->GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
	}
}
