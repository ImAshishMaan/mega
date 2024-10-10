#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

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

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);
	
private:

	
};
