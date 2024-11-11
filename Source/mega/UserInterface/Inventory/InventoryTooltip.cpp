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
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		break;
	case EItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item")); 
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
		default: ;
	}

	ItemType->SetText(ItemBeingHovered->ItemTextData.Name); 
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisticsData.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisticsData.ArmorRating));
	UsageText->SetText(ItemBeingHovered->ItemTextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->ItemTextData.Description);
	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));
	SellValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatisticsData.SellValue));

	if(ItemBeingHovered->ItemNumericData.bIsStackable) {
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->ItemNumericData.MaxStackSize));
	} else {
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
