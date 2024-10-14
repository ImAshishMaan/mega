#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UHealthBarWIdget;
class UAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class MEGA_API ASAICharacter : public ACharacter {
	GENERATED_BODY()

public:
	ASAICharacter();

protected:
	void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage);
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UHealthBarWIdget* ActiveHealthBar;

	
};
