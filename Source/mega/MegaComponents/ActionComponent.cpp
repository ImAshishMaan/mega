#include "ActionComponent.h"
#include "mega/Actions/MAction.h"

UActionComponent::UActionComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay() {
	Super::BeginPlay();

	for(TSubclassOf<UMAction> Action: DefaultActions) {
		AddAction(GetOwner(), Action);
	}
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, DebugMsg);
}

void UActionComponent::AddAction(AActor* Instigator, TSubclassOf<UMAction> ActionClass) {
	if(!ensure(ActionClass)) return;

	UMAction* NewAction = NewObject<UMAction>(this, ActionClass);
	if(ensure(NewAction)) {
		Actions.Add(NewAction);
		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator))) {
			NewAction->StartAction(Instigator);
		}
	}
}

void UActionComponent::RemoveAction(UMAction* ActionToRemove) {
	if(!ensure(ActionToRemove && ActionToRemove->IsRunning())) return;

	Actions.Remove(ActionToRemove);
}

bool UActionComponent::StartActionByName(AActor* Instigator, FName ActionName) {
	for(UMAction* Action: Actions) {
		if(Action && Action->ActionName == ActionName) {
			if(Action->CanStart(Instigator)) {
				Action->StartAction(Instigator);
				return true;
			} else {
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMsg);
				continue;
			}
		}
	}
	return false;
}

bool UActionComponent::StopActionByName(AActor* Instigator, FName ActionName) {
	for(UMAction* Action: Actions) {
		if(Action && Action->ActionName == ActionName) {
			Action->StopAction(Instigator);
			return true;
		} else {
			FString FailedMsg = FString::Printf(TEXT("Failed to stop: %s"), *ActionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FailedMsg);
			continue;
		}
	}
	return false;
}
