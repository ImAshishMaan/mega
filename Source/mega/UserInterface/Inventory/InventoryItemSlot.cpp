#include "InventoryItemSlot.h"
#include "InventoryTooltip.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "mega/Items/ItemBase.h"

void UInventoryItemSlot::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if(TooltipClass) {
		UInventoryTooltip* Tooltip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
		Tooltip->InventorySlotBeingHovered = this;
		SetToolTip(Tooltip);
	}
}

void UInventoryItemSlot::NativeConstruct() {
	Super::NativeConstruct();

	if(ItemReference) {
		switch(ItemReference->ItemQuality) {
		case EItemQuality::Shoddy:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EItemQuality::Common:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EItemQuality::Quality:
			ItemBorder->SetBrushColor(FLinearColor::Green);
			break;
		case EItemQuality::Masterwork:
			ItemBorder->SetBrushColor(FLinearColor::Blue);
			break;
		case EItemQuality::Grandmaster:
			ItemBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f)); // orange
			break;
			default: ;
		}

		// Set Item Icon
		ItemIcon->SetBrushFromTexture(ItemReference->ItemAssetData.Icon);
		if(ItemReference->ItemNumericData.bIsStackable) {
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
		} else {
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent) {
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) {
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) {
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
