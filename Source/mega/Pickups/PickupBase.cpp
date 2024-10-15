#include "PickupBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"

APickupBase::APickupBase() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	SphereOverlapComp = CreateDefaultSubobject<USphereComponent>("SphereOverlapComp");
	SphereOverlapComp->SetupAttachment(RootComponent);
	SphereOverlapComp->SetSphereRadius(20.0f);
	SphereOverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereOverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereOverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	PickupMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("PickupMeshComp");
	PickupMeshComp->SetupAttachment(SphereOverlapComp);
	PickupMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMeshComp->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	PickupEffectComp = CreateDefaultSubobject<UNiagaraComponent>("PickupEffectComp");
	PickupEffectComp->SetupAttachment(RootComponent);
	
}

void APickupBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if(PickupMeshComp) {
		PickupMeshComp->AddLocalRotation(FRotator(0.0f, BaseTurnRate * DeltaTime, 0.0f));
	}
}

void APickupBase::BeginPlay() {
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(BindOverlapTimer, this, &APickupBase::BindOverlapTimerFinished, BindOverlapTime);
}

void APickupBase::BindOverlapTimerFinished() {
	SphereOverlapComp->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnSphereOverlap);
}

void APickupBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if(PickupSound) {
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
	if(PickupEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, GetActorLocation());
	}
	Destroy();
}


