#include "ProjectilePoolManager.h"
#include "Engine/World.h"
#include "mega/Weapon/Projectile.h"

// Define the static instance of the class
AProjectilePoolManager* AProjectilePoolManager::Instance = nullptr;  // Make sure this is at the top of your .cpp file

AProjectilePoolManager::AProjectilePoolManager() {
	PrimaryActorTick.bCanEverTick = false;
}

void AProjectilePoolManager::BeginPlay() {
	Super::BeginPlay();
	World = GetWorld();

	// Set this instance as the singleton if not already set
	if (Instance == nullptr) {
		Instance = this;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AProjectilePoolManager already exists!"));
	}
}

// Singleton accessor
AProjectilePoolManager* AProjectilePoolManager::GetInstance() {
	return Instance;
}

AProjectile* AProjectilePoolManager::GetProjectile() {
	// Check if there is an inactive projectile in the pool
	for (AProjectile* Projectile : ProjectilePool) {
		if (!Projectile->IsActive()) {
			return Projectile;
		}
	}

	// Pool is empty or no inactive projectiles available, spawn a new one
	if (ProjectileClass && World) {
		FActorSpawnParameters SpawnParams;
		AProjectile* NewProjectile = World->SpawnActor<AProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (NewProjectile) {
			NewProjectile->SetActorEnableCollision(false);
			NewProjectile->SetActorHiddenInGame(true);
			ProjectilePool.Add(NewProjectile); // Add it to the pool for future reuse
			return NewProjectile;
		}
	}

	return nullptr; // If something goes wrong
}

void AProjectilePoolManager::ReturnProjectile(AProjectile* Projectile) {
	Projectile->ResetProjectile(); // Reset any necessary state for reuse
}
