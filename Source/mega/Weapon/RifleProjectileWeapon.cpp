#include "RifleProjectileWeapon.h"

#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "mega/PoolingSystem/ProjectilePoolManager.h"

void ARifleProjectileWeapon::Fire(const FVector& HitTarget) {
	Super::Fire(HitTarget);

	// spawn bullet

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket) {
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();

		AProjectilePoolManager* PoolManager = AProjectilePoolManager::GetInstance();
		if(PoolManager && InstigatorPawn) {
			AProjectile* Projectile = PoolManager->GetProjectile();
			if(Projectile) {
				Projectile->SetActorLocation(SocketTransform.GetLocation());
				Projectile->SetActorRotation(TargetRotation);
				Projectile->SetOwner(GetOwner());
				Projectile->SetInstigator(InstigatorPawn);

				// Reinitialize movement
				UProjectileMovementComponent* ProjectileMovement = Projectile->GetProjectileMovement();
				if(ProjectileMovement) {
					// Set the velocity in the direction of the target
					ProjectileMovement->Velocity = TargetRotation.Vector() * 20000.0f;
					ProjectileMovement->Activate(true); // Ensure the movement component is activated
				}

				// Activate the projectile
				Projectile->ActivateProjectile();
			}
		}
		/*checkf(ProjectileClass, TEXT("Projectile class not set."));
		if(ProjectileClass && InstigatorPawn) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = InstigatorPawn;
			SpawnParams.Owner = GetOwner();

			UWorld* World = GetWorld();
			if(World) {
				AProjectile* Projectile = World->SpawnActor<AProjectile>(
					ProjectileClass,
					SocketTransform.GetLocation(),
					TargetRotation,
					SpawnParams
				);
			}
		}*/
	}
}
