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

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit) {
	ShowImpactParticles();
	Destroy();
}

void AProjectile::ShowImpactParticles() {
	if(ImpactParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, GetActorLocation(), FRotator(0.f), true);
	}
	if(ImpactSound) {
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}
