#include "MainMenu.h"

#include "Inventory/ItemDragDropOperation.h"
#include "mega/Items/ItemBase.h"

void UMainMenu::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct() {
	Super::NativeConstruct();

	PlayerCharacter = Cast<AMegaCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {

	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	
	if(PlayerCharacter && ItemDragDrop->SourceItem) {
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}
