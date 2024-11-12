#pragma once

#include "CoreMinimal.h"
#include "mega/Character/MegaCharacter.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UENUM()
enum class EInteractionType : uint8 {
	Pickup UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"),
	Container UMETA(DisplayName = "Container"),
	
};

USTRUCT()
struct FInteractableData {
	GENERATED_USTRUCT_BODY()

	FInteractableData() :
	InteractionType(EInteractionType::Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(1),
	InteractionDuration(0.0f)
	{};

	UPROPERTY(EditInstanceOnly)
	EInteractionType InteractionType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// only for pickup
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	// Used for things like doors, gate etc, that require a duration
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class MEGA_API IInteractionInterface {
	GENERATED_BODY()

public:

	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(AMegaCharacter* PlayerCharacter);

	FInteractableData InteractableData;
	
};
