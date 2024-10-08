#include "RifleProjectileWeapon.h"

#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void ARifleProjectileWeapon::Fire(const FVector& HitTarget) {
	Super::Fire(HitTarget);

	// spawn bullet

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket) {
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotaion = ToTarget.Rotation();
		checkf(ProjectileClass, TEXT("Projectile class not set."));
		if(ProjectileClass && InstigatorPawn) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = InstigatorPawn;
			SpawnParams.Owner = GetOwner();

			UWorld* World = GetWorld();
			if(World) {
				AProjectile* Projectile = World->SpawnActor<AProjectile>(
					ProjectileClass,
					SocketTransform.GetLocation(),
					TargetRotaion,
					SpawnParams
				);
			}
		}
	}
}
