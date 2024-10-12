#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectilePoolManager.generated.h"

class AProjectile;

UCLASS()
class MEGA_API AProjectilePoolManager : public AActor {
	GENERATED_BODY()

public:
	AProjectilePoolManager();
	
	static AProjectilePoolManager* GetInstance();  

	void ReturnProjectile(AProjectile* Projectile);
	AProjectile* GetProjectile();
	
protected:
	virtual void BeginPlay() override;

	// Static reference to the singleton instance
	static AProjectilePoolManager* Instance;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile Pool")
	TArray<AProjectile*> ProjectilePool;

	UWorld* World;
};
