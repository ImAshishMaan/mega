#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

class AMegaCharacter;
class AMegaPlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MEGA_API UAttributeComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UAttributeComponent();
	friend class AMegaCharacter;

protected:
	/*
	 * Attributes System Initialization
	 * It's called from MegaCharacter.cpp (It's like a BeginPlay)
	 * Cannot use BeginPlay because montage BeginPlay might be called before character able to initialize some values in AttributeComponent
	 */
	void InitializeAttributesSystem();
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth;
	
private:
	UPROPERTY()
	AMegaCharacter* MegaCharacter;

	UPROPERTY()
	AMegaPlayerController* MegaPlayerController;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE bool IsAlive() const { return Health > 0.0f; }
	
};
