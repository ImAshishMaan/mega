#include "SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "mega/HUD/HealthBarWIdget.h"
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
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* SeenPawn) {
	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController) {
		UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

		BlackboardComp->SetValueAsObject("TargetActor", SeenPawn);

		DrawDebugString(GetWorld(), SeenPawn->GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage) {

	if(NewHealth >= 0.0f) {
		if(ActiveHealthBar == nullptr && HealthBarWidgetClass) {
			ActiveHealthBar = CreateWidget<UHealthBarWIdget>(GetWorld(), HealthBarWidgetClass);

			if(ActiveHealthBar) {
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
	}
}
