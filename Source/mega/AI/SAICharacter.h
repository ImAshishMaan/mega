#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

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
	
private:

	
};
