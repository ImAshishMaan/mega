#include "Action_MagicProjectileAttack.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UAction_MagicProjectileAttack::UAction_MagicProjectileAttack() {
	HandSocketName = "MagicFireSocket";
	AttackAnimDelay = 0.3f;
}

void UAction_MagicProjectileAttack::StartAction_Implementation(AActor* Instigator) {
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(Character) {
		Character->PlayAnimMontage(AttackAnim);
		if(CastingEffect) {
			UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
		}
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Instigator);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}


void UAction_MagicProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter) {
	if(ensure(ProjectileClass)) {
		/*FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FHitResult Hit;
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000.0f);

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FRotator ProjRotation;
		// true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
		if(GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params)) {
			// Adjust location to end up at crosshair look-at
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		} else {
			// Fall-back since we failed to find any blocking hit
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);*/
		FVector2D ViewportSize;
		FHitResult TraceHitResult;
		if(GEngine && GEngine->GameViewport) {
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Get the crosshair location in the middle of the screen
		FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);
		FVector CrosshairWorldPosition;
		FVector CrosshairWorldDirection;

		// Deproject the crosshair screen location to world space
		bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
			UGameplayStatics::GetPlayerController(this, 0),
			CrosshairLocation,
			CrosshairWorldPosition,
			CrosshairWorldDirection
		);

		if(bScreenToWorld) {
			// Start trace from the crosshair world position or character's location
			FVector Start = CrosshairWorldPosition;

			// The trace end point far ahead in the direction of the crosshair
			FVector End = Start + CrosshairWorldDirection * 80000.0f;

			// Ignore the character for the trace
			FCollisionQueryParams TracerParams;
			TracerParams.AddIgnoredActor(InstigatorCharacter);

			// Perform the trace
			GetWorld()->LineTraceSingleByChannel(
				TraceHitResult,
				Start,
				End,
				ECollisionChannel::ECC_Visibility,
				TracerParams
			);

			// If no hit was found, set the trace hit result's ImpactPoint to the end point
			if(!TraceHitResult.bBlockingHit) {
				TraceHitResult.ImpactPoint = End; // This ensures the bullet goes in the crosshair direction
			}

			const USkeletalMeshSocket* MagicAttackSocket = InstigatorCharacter->GetMesh()->GetSocketByName("MagicFireSocket");
			if(MagicAttackSocket) {
				FTransform SocketTransform = MagicAttackSocket->GetSocketTransform(InstigatorCharacter->GetMesh());

				const FVector ToTarget = TraceHitResult.ImpactPoint - SocketTransform.GetLocation();
				FRotator TargetRotation = ToTarget.Rotation();
				checkf(ProjectileClass, TEXT("Projectile class not set."));
				if(ProjectileClass && InstigatorCharacter) {
					FActorSpawnParameters SpawnParams;
					SpawnParams.Instigator = InstigatorCharacter;
					SpawnParams.Owner = InstigatorCharacter;

					UWorld* World = GetWorld();
					if(World) {
						World->SpawnActor<AActor>(
							ProjectileClass,
							SocketTransform.GetLocation(),
							TargetRotation,
							SpawnParams
						);
					}
				}
			}
		}

		StopAction(InstigatorCharacter);
	}
}
