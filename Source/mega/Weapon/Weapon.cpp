#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "mega/Character/MegaCharacter.h"

AWeapon::AWeapon() {
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);


	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	
}

void AWeapon::BeginPlay() {
	Super::BeginPlay();
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// set this actor in the overlapping actor in character class
	UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlap"));

	AMegaCharacter* MegaCharacter = Cast<AMegaCharacter>(OtherActor);
	if(MegaCharacter) {
		
	}
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	// set this actor to null in the overlapping actor in character class
	UE_LOG(LogTemp, Warning, TEXT("OnSphereEndOverlap"));
	
	AMegaCharacter* MegaCharacter = Cast<AMegaCharacter>(OtherActor);
	if(MegaCharacter) {
		
	}
}


void AWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AWeapon::Fire(const FVector& HitTarget) {
	// Fire animation
	if(FireAnimation) {
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}

	// bullet shells spawn

	// reduce ammo
	SpendRound();
}
void AWeapon::SpendRound() {
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	// Set HUD Ammo
}
