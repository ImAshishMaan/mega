#include "ItemPickups.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "mega/Items/ItemBase.h"

AItemPickups::AItemPickups() {
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}


void AItemPickups::BeginPlay() {
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void AItemPickups::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity) {
	if(ItemDataTable && !DesiredItemID.IsNone()) {
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;

		InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		UpdateInteractableData();
		
	}
}

void AItemPickups::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity) {
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->ItemNumericData.Weight = ItemToDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemToDrop->ItemAssetData.Mesh);
	
	UpdateInteractableData();
	
}

void AItemPickups::UpdateInteractableData() {
	InstanceInteractableData.InteractionType = EInteractionType::Pickup;
	InstanceInteractableData.Action = ItemReference->ItemTextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->ItemTextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;

	InteractableData = InstanceInteractableData;
}


void AItemPickups::BeginFocus() {
	if(PickupMesh) {
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void AItemPickups::EndFocus() {
	if(PickupMesh) {
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void AItemPickups::Interact(AMegaCharacter* PlayerCharacter) {
	if(PlayerCharacter) {
		TakePickup(PlayerCharacter);
	}
}


void AItemPickups::TakePickup(const AMegaCharacter* Taker) {
	if(!IsPendingKillPending()) {
		if(ItemReference) {
			/*if(UInventoryComponenet* PlayerInventory = Taker->GetInventory()) {
				
			}*/

			// try to add item to player inventory
			// based on result of that we add operation
			// adjust or destroy item
		}
	}
}

void AItemPickups::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangePropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if(ChangePropertyName == GET_MEMBER_NAME_CHECKED(AItemPickups, DesiredItemID)) {
		if(ItemDataTable) {
			const FString ContextString{DesiredItemID.ToString()};

			if(const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString())) {
				PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);
			}
		}
	}
}



