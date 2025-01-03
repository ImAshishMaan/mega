#include "InventoryPanel.h"
#include "InventoryItemSlot.h"
#include "ItemDragDropOperation.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "mega/Character/MegaCharacter.h"
#include "mega/Items/ItemBase.h"
#include "mega/MegaComponents/InventoryComponent.h"

void UInventoryPanel::NativeOnInitialized() {
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AMegaCharacter>(GetOwningPlayerPawn());
	if(PlayerCharacter) {
		InventoryReference = PlayerCharacter->GetInventory();
		if(InventoryReference) {
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryPanel::SetInfoText() const {
	WeightInfo->SetText(FText::FromString(FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/" + FString::SanitizeFloat(InventoryReference->GetWeightCapacity())));
	CapacityInfo->SetText(FText::FromString(FString::FromInt(InventoryReference->GetInventoryContents().Num()) + "/" + FString::FromInt(InventoryReference->GetSlotsCapacity())));
}

void UInventoryPanel::RefreshInventory() {
	if(InventoryReference && InventorySlotClass) {
		InventoryWrapBox->ClearChildren();

		for(UItemBase* const& InventoryItem: InventoryReference->GetInventoryContents()) {
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);

			InventoryWrapBox->AddChildToWrapBox(ItemSlot);
		}
		SetInfoText();
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."))

		// returning true will stop the drop operation at this widget
		return true;
	}
	// returning false will cause the drop operation to fall through to underlying widgets (if any)
	return false;
}
