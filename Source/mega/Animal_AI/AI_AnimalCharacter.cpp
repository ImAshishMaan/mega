#include "AI_AnimalCharacter.h"
#include "AI_AnimalController.h"

AAI_AnimalCharacter::AAI_AnimalCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	AnimalMesh = GetMesh();

	HungryAmount = MaxHungryAmount;
	HungerDecayRate = 1;
	
}

void AAI_AnimalCharacter::BeginPlay() {
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(HungryTimerHandle, this, &AAI_AnimalCharacter::StartHungryTimer, 1.0f, true);
	GetWorld()->GetTimerManager().SetTimer(RestingTimerHandle, this, &AAI_AnimalCharacter::StartRestingTimer, 1.0f, true);
}

void AAI_AnimalCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


void AAI_AnimalCharacter::StartHungryTimer() {
	if (HungryAmount > 0) {
		HungryAmount--;
	}

	if(HungryAmount <= 0 && IsHungry == false) {
		HungryAmount = 0;
		OnHungryTimerFinished();
	}
}

void AAI_AnimalCharacter::OnHungryTimerFinished() {
	AAI_AnimalController* AIC = Cast<AAI_AnimalController>(GetController());
	if(AIC) {
		UpdateState(AIState::Hungry, true);
		AIC->UpdateHunger(IsHungry);
	}
}

void AAI_AnimalCharacter::StartEatingTimer() {
	UpdateState(AIState::Eating, true);
	GetWorld()->GetTimerManager().SetTimer(EatingTimerHandle, this, &AAI_AnimalCharacter::OnEatingTimerFinished, 4.0f, false);
}

void AAI_AnimalCharacter::OnEatingTimerFinished() {
	UpdateState(AIState::Eating, false);
	HungryAmount = MaxHungryAmount;
	UpdateState(AIState::Hungry, false);
	AAI_AnimalController* AIC = Cast<AAI_AnimalController>(GetController());
	if(AIC) {
		AIC->UpdateHunger(IsHungry);
	}
}

void AAI_AnimalCharacter::StartRestingTimer() {
	if(RestingTime > 0) {
		RestingTime--;
	}

	if(RestingTime <= 0 and IsResting == false) {
		RestingTime = 0;
		UpdateState(AIState::Resting, true);
		AAI_AnimalController* AIC = Cast<AAI_AnimalController>(GetController());
		if(AIC) {
			AIC->UpdateEating(IsResting);
		}
		GetWorld()->GetTimerManager().SetTimer(RestingTimerStopHandle, this, &AAI_AnimalCharacter::OnRestingTimerFinished, 10.0f, false);
	}
}
void AAI_AnimalCharacter::OnRestingTimerFinished() {
	UpdateState(AIState::Resting, false);
	RestingTime = 300;
}

void AAI_AnimalCharacter::UpdateState(AIState NewState, bool what) {
	switch(NewState) {
		case AIState::FreeMovement:
			SetupFreeMovementState();
			break;
		case AIState::Eating:
			SetupEatingState(what);
			break;
		case AIState::Hungry:
			SetupHungryState(what);
			break;
		case AIState::Resting:
			SetupRestingState(what);
			break;
		case AIState::Dead:
			SetupDeadState();
			break;
		default:
			break;
	}
}

void AAI_AnimalCharacter::SetupFreeMovementState() {
	IsEating = false;
	IsHungry = false;
}

void AAI_AnimalCharacter::SetupHungryState(bool Hungry) {
	IsHungry = Hungry;
}

void AAI_AnimalCharacter::SetupEatingState(bool Eating) {
	IsEating = Eating;
	if(Eating == false) { // Eating is finished
		RestingTime = 7;
	}
}

void AAI_AnimalCharacter::SetupRestingState(bool Resting) {
	IsResting = Resting;
}

void AAI_AnimalCharacter::SetupDeadState() {
	// TODO
}

