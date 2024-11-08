#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "mega/Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class MEGA_API AInterfaceTestActor : public AActor, public IInteractionInterface {
	GENERATED_BODY()

public:
	AInterfaceTestActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(AMegaCharacter* PlayerCharacter) override;
	
};
