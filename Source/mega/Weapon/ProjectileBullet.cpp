#include "ProjectileBullet.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

	if (OtherActor && OtherActor != this && OtherActor != BulletOwner) {
		if (OtherComp && OtherComp->IsSimulatingPhysics() && OtherComp->GetMass() > 0.0f) {
			// Remember to set object mass in the Physics Asset
			float Mass = OtherComp->GetMass();
			float ImpulseStrength = 20000.0f;  
			FVector ImpulseDirection = Hit.ImpactNormal * -1.0f;

			FVector Impulse = ImpulseDirection * ImpulseStrength;
			Impulse = Impulse / Mass;
			
			OtherComp->AddImpulseAtLocation(Impulse, Hit.ImpactPoint);
		}
	}
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
