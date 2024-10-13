#include "AttributeComponent.h"


UAttributeComponent::UAttributeComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.0f;
	MaxHealth = 100.0f;
}

bool UAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Damage) {
	if(!GetOwner()->CanBeDamaged() && Damage < 0.0f) {
		return false;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, MaxHealth);

	// If actor is dead call OnActorDead in GM

	return true;
}

UAttributeComponent* UAttributeComponent::GetAttributes(AActor* FromActor) {
	if(FromActor) {
		return FromActor->FindComponentByClass<UAttributeComponent>();
	}
	return nullptr;
}


