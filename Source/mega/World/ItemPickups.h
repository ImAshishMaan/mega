#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "mega/Interfaces/InteractionInterface.h"
#include "ItemPickups.generated.h"

class UItemBase;
class UDataTable;

UCLASS()
class MEGA_API AItemPickups : public AActor, public IInteractionInterface {
	GENERATED_BODY()

public:
	//===============================================================================
	// Properties & Variables
	//===============================================================================
	


	//===============================================================================
	// Functions
	//===============================================================================
	
	AItemPickups();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; }

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	
protected:
	//===============================================================================
	// Properties & Variables
	//===============================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	//===============================================================================
	// Functions
	//===============================================================================
	virtual void BeginPlay() override;
	
	virtual void Interact(AMegaCharacter* PlayerCharacter) override;
	void UpdateInteractableData();

	void TakePickup(const AMegaCharacter* Taker);
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};


