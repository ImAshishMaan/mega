#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "mega/mega.h"
#include "mega/PoolingSystem/ProjectilePoolManager.h"
#include "Sound/SoundCue.h"

AProjectile::AProjectile() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 30000.0f;
	ProjectileMovement->MaxSpeed = 30000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AProjectile::BeginPlay() {
	Super::BeginPlay();

	if(Tracer) {
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			CollisionBox,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}
	CollisionBox->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit) {

	ShowImpactParticles(HitComp, OtherActor);
	PlayImpactSound();
	//Destroy();

	AProjectilePoolManager* PoolManager = AProjectilePoolManager::GetInstance();
	if (PoolManager) {
		PoolManager->ReturnProjectile(this);
	}
}

void AProjectile::ResetProjectile() {
	bInUse = false;

	// Reset movement and state
	if (ProjectileMovement) {
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Velocity = FVector::ZeroVector;  // Reset velocity
	}

	SetActorHiddenInGame(true);   // Hide the projectile
	SetActorEnableCollision(false);  // Disable collision

	SetActorLocation(FVector::ZeroVector); 
}

void AProjectile::ActivateProjectile() {
	bInUse = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	if (ProjectileMovement) {
		ProjectileMovement->Activate(true);  // Reactivate the movement component
	}
}

bool AProjectile::IsActive() const {
	return bInUse;
}

void AProjectile::PlayImpactSound() {
	if(ImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void AProjectile::ShowImpactParticles(UPrimitiveComponent* HitComponent, AActor* OtherActor) {
	UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if(MeshComp) {
		UMaterialInterface* Material = MeshComp->GetMaterial(0);
		if(Material) {
			UPhysicalMaterial* PhysicalMaterialFromVisual = Material->GetPhysicalMaterial();
			if(PhysicalMaterialFromVisual) {
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicalMaterialFromVisual);
				if(SurfaceType == SurfaceType_Metal && MetalImpactParticle) {
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MetalImpactParticle, GetActorLocation(), FRotator(0.f), true);
				} else if(SurfaceType == SurfaceType_Wood && WoodImpactParticle) {
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WoodImpactParticle, GetActorLocation(), FRotator(0.f), true);
				}
			}
		} else {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MetalImpactParticle, GetActorLocation(), FRotator(0.f), true); // Default particle
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Projectile: No StaticMeshComponent found on hit actor!"));
	}
}
