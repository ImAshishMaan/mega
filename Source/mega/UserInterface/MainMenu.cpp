#include "MainMenu.h"

void UMainMenu::NativeOnInitialized() {
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct() {
	Super::NativeConstruct();
	
	PlayerCharacter = Cast<AMegaCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// cast operation to item drag drop operation
}
