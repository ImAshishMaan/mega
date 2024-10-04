#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "mega/Interfaces/AnimationInterface.h"
#include "MegaCharacter.generated.h"

UCLASS()
class MEGA_API AMegaCharacter : public ACharacter, public IAnimationInterface {
	GENERATED_BODY()

public:
	AMegaCharacter();
	void SetWalkState();
	void SetJogState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
