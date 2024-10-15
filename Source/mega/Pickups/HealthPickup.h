#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API AHealthPickup : public APickupBase {
	GENERATED_BODY()

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
