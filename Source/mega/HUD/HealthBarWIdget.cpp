#include "HealthBarWIdget.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "mega/MegaComponents/AttributeComponent.h"


void UHealthBarWIdget::NativeConstruct() {
	Super::NativeConstruct();

	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AttachedActor);
	if(AttributeComp) {
		AttributeComp->OnHealthChanged.AddDynamic(this, &UHealthBarWIdget::UpdateHealth);
	}
}

void UHealthBarWIdget::UpdateHealth(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage) {
	HealthBar->SetPercent(NewHealth/MaxHealth);

	if(NewHealth <= 0.0f) {
		RemoveFromParent();
	}
}
