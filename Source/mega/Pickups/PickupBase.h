#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class MEGA_API APickupBase : public AActor {
	GENERATED_BODY()
	
public:
	APickupBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.0f;
	
private:

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* SphereOverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	UStaticMeshComponent* PickupMeshComp;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	UNiagaraComponent* PickupEffectComp;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = "Pickup")
	USoundCue* PickupSound;

	FTimerHandle BindOverlapTimer;
	float BindOverlapTime = 0.5f;
	void BindOverlapTimerFinished();
	
};
