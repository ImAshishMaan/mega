#include "MAction.h"

UActionComponent* UMAction::GetOwningComponent() const {
	return Cast<UActionComponent>(GetOuter());
}

UWorld* UMAction::GetWorld() const {
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if(Comp) {
		return Comp->GetWorld();
	}
	return nullptr;
}

void UMAction::StartAction_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Log, TEXT("Action Started: %s"), *GetNameSafe(this));

	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);
}

void UMAction::StopAction_Implementation(AActor* Instigator) {
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	UActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);
}

bool UMAction::CanStart_Implementation(AActor* Instigator) {
	UActionComponent* Comp = GetOwningComponent();

	if(Comp->ActiveGameplayTags.HasAny(BlockedTags)) {
		return false;
	}
	return true;
}

bool UMAction::IsRunning() const {
	return bIsRunning;
}
