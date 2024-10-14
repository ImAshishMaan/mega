#include "AbilityComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "mega/Character/MegaCharacter.h"
#include "mega/Weapon/Projectile.h"

UAbilityComponent::UAbilityComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}
void UAbilityComponent::InitializeAbilitySystem() {
	
}

void UAbilityComponent::MagicAttack(const FVector& HitTarget) {
	if(AttackAnim) {
		MegaCharacter->PlayAnimMontage(AttackAnim);
	}
	
	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AttackDelay_Elapsed", HitTarget);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
}

void UAbilityComponent::AttackDelay_Elapsed(const FVector& HitTarget) {
	if(CastingEffect) {
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, MegaCharacter->GetMesh(), "MagicFireSocket", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	}
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MagicAttackSocket = MegaCharacter->GetMesh()->GetSocketByName("MagicFireSocket");
	if(MagicAttackSocket) {
		FTransform SocketTransform = MagicAttackSocket->GetSocketTransform(MegaCharacter->GetMesh());

		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
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
					TargetRotation,
					SpawnParams
				);
			}
		}
	}
}




