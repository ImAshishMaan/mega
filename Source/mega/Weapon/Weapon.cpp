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
		MegaCharacter->SetOverlappingWeapon(this);
	}
	
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	// set this actor to null in the overlapping actor in character class
	UE_LOG(LogTemp, Warning, TEXT("OnSphereEndOverlap"));
	
	AMegaCharacter* MegaCharacter = Cast<AMegaCharacter>(OtherActor);
	if(MegaCharacter) {
		MegaCharacter->SetOverlappingWeapon(nullptr);
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

void AWeapon::Dropped() {
	SetWeaponState(EWeaponState::EWS_Dropped);
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	WeaponMesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr);
	// remove all other previous character object instances. i.e. Set AMegaCharacter to nullptr

}

void AWeapon::SetWeaponState(EWeaponState State) {
	WeaponState = State;
	OnWeaponStateSet();
}

void AWeapon::OnWeaponStateSet() {

	switch(WeaponState) {
		case EWeaponState::EWS_Equipped:
			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			WeaponMesh->SetSimulatePhysics(false);
			WeaponMesh->SetEnableGravity(false);
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EWeaponState::EWS_Dropped:
			CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
			WeaponMesh->SetSimulatePhysics(true);
			WeaponMesh->SetEnableGravity(true);
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		    WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
			WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			WeaponMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
			break;
		default: ;
	}
}

void AWeapon::SpendRound() {
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	// Set HUD Ammo
}
