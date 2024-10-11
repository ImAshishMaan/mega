#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "mega/mega.h"
#include "Sound/SoundCue.h"

AProjectile::AProjectile() {
	PrimaryActorTick.bCanEverTick = true;


	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);
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

void AProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit) {

	ShowImpactParticles(HitComp, OtherActor);
	PlayImpactSound();
	Destroy();
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
