#include "MagicProjectile.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "mega/MegaComponents/ActionComponent.h"
#include "mega/MegaComponents/AttributeComponent.h"

AMagicProjectile::AMagicProjectile() {
	SphereComp->SetSphereRadius(10.0f);
	DamageAmount = 35.0f;
}


void AMagicProjectile::OnActorHIt(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
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
	SpawnDecal(Hit);
	Super::OnActorHIt(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AMagicProjectile::SpawnDecal(const FHitResult& ImpactResult) {
	if(DecalMaterial) {
		float RandomValue = FMath::FRandRange(25.f, 45.f);
		FVector DecalSize = FVector(RandomValue, RandomValue, RandomValue);
		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
			this,
			DecalMaterial,
			DecalSize,
			ImpactResult.Location,
			ImpactResult.Normal.Rotation(),
			4.0f
		);

		if(DecalComponent) {
			DecalComponent->SetFadeScreenSize(0.001f);
			//DecalComponent->SetFadeOut(2.f, 2.f); // Not working for some reason
		}
	}
}

