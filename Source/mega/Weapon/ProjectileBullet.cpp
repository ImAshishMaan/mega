#include "ProjectileBullet.h"

#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
		AController* OwnerController = BulletOwner->GetController();
		if(OwnerController) {
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, BulletOwner, UDamageType::StaticClass());
		}
	}
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
