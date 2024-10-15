#include "HealthPickup.h"
#include "mega/MegaComponents/AttributeComponent.h"

void AHealthPickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(OtherActor);
	if(AttributeComp) {
		AttributeComp->ApplyHealthChange(OtherActor, -AttributeComp->GetMaxHealth());
	}
	
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
