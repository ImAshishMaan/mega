#include "MagicProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

AMagicProjectileBase::AMagicProjectileBase() {
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectComp"));
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 8000;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	ForceComp->SetupAttachment(SphereComp);

}

void AMagicProjectileBase::BeginPlay() {
	Super::BeginPlay();
	
	SphereComp->IgnoreActorWhenMoving(GetOwner(), true);
	SphereComp->OnComponentHit.AddDynamic(this, &AMagicProjectileBase::OnActorHIt);
}

void AMagicProjectileBase::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

void AMagicProjectileBase::OnActorHIt(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	Explode();
}


void AMagicProjectileBase::Explode_Implementation() {
	if(ImpactVFX)
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	Destroy();
}

