#include "MagicProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "mega/MegaComponents/ActionComponent.h"
#include "mega/MegaComponents/AttributeComponent.h"

AMagicProjectile::AMagicProjectile() {
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnActorOverlap);

	DamageAmount = 35.0f;
}

void AMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	UActionComponent* ActionComp = UActionComponent::GetActionComponent(OtherActor);
	if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag)) {
		MovementComp->Velocity = -MovementComp->Velocity; // Will work if bRotationFollowsVelocity = true;

		SetInstigator(Cast<APawn>(OtherActor));
		return;
	}

	ACharacter* BulletOwner = Cast<ACharacter>(GetOwner());
	if(BulletOwner) {
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(OtherActor);
		if(AttributeComp) {
			AttributeComp->ApplyHealthChange(BulletOwner, DamageAmount);
		}
	}
	Explode();
}
