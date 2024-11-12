#include "InventoryTooltip.h"
#include "InventoryItemSlot.h"
#include "Components/TextBlock.h"
#include "mega/Items/ItemBase.h"

void UInventoryTooltip::NativeConstruct() {
	Super::NativeConstruct();

	UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch(ItemBeingHovered->ItemType) {
	case EItemType::Armor:
		break;
	case EItemType::Weapon:
		break;
	case EItemType::Shield:
		break;
	case EItemType::Spell:
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		break;
	case EItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item")); 
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		break;
		default: ;
	}

	ItemName->SetText(ItemBeingHovered->ItemTextData.Name); 
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisticsData.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisticsData.ArmorRating));
	UsageText->SetText(ItemBeingHovered->ItemTextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->ItemTextData.Description);
	StackWeight->SetText(FText::FromString("Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())));

	if(ItemBeingHovered->ItemNumericData.bIsStackable) {
		MaxStackSize->SetText(FText::FromString("Max stack size: " + FString::FromInt(ItemBeingHovered->ItemNumericData.MaxStackSize)));
	} else {
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
