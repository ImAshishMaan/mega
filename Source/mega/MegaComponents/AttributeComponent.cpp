#include "AttributeComponent.h"

#include "mega/Character/MegaCharacter.h"
#include "mega/PlayerController/MegaPlayerController.h"

UAttributeComponent::UAttributeComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	Health = 100.0f;
	MaxHealth = 100.0f;
}

void UAttributeComponent::InitializeAttributesSystem() {
	if(MegaCharacter) {
		MegaCharacter->OnTakeAnyDamage.AddDynamic(this, &UAttributeComponent::OnTakeAnyDamage);
	}
}

void UAttributeComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser) {

	float OldHealth = Health;
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
	// can call OnHealthChanged event here to send data
	MegaPlayerController = MegaPlayerController == nullptr ? Cast<AMegaPlayerController>(MegaCharacter->Controller) : MegaPlayerController;
	if(MegaPlayerController) {
		MegaPlayerController->SetHUDHealth(Health, MaxHealth);
	}
	// call HUD health update
	// call hit react montage here
}
