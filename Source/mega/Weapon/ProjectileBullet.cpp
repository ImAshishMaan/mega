#include "ProjectileBullet.h"

#include "Components/DecalComponent.h"
#include "Field/FieldSystemActor.h"
#include "Field/FieldSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "mega/MegaComponents/AttributeComponent.h"

AProjectileBullet::AProjectileBullet() {
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->InitialSpeed = 30000.0f;
	ProjectileMovement->MaxSpeed = 30000.0f;
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit) {
	ACharacter* BulletOwner = Cast<ACharacter>(GetOwner());
	if(BulletOwner) {
		UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(OtherActor);
		if(AttributeComp) {
			AttributeComp->ApplyHealthChange(BulletOwner, Damage);
		}
	}

	/*if(OtherActor && OtherActor != this && OtherActor != BulletOwner) {
		if(OtherComp && OtherComp->IsSimulatingPhysics() && OtherComp->GetMass() > 0.0f) {
			// Remember to set object mass in the Physics Asset
			float Mass = OtherComp->GetMass();
			float ImpulseStrength = 20000.0f;
			FVector ImpulseDirection = Hit.ImpactNormal * -1.0f;

			FVector Impulse = ImpulseDirection * ImpulseStrength;
			Impulse = Impulse / Mass;

			OtherComp->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
		}
	}*/

	if(OtherComp && OtherComp->IsSimulatingPhysics()) {

		if(FieldSystem) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(Hit.ImpactPoint);
			SpawnTransform.SetRotation(FQuat::Identity);
			SpawnTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

			// Spawn the field actor
			AActor* SpawnedField = GetWorld()->SpawnActor<AActor>(FieldSystem, SpawnTransform, SpawnParams);
			if(SpawnedField) {
				SpawnedField->SetLifeSpan(.1f);
			}
		}
	}

	SpawnBulletHoles(Hit);

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileBullet::SpawnBulletHoles(const FHitResult& ImpactResult) {
	if(DecalMaterial) {
		float RandomValue = FMath::FRandRange(7.f, 12.f);
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
