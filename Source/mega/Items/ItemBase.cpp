#include "ItemBase.h"
#include "mega/MegaComponents/InventoryComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(true) {
	
}

void UItemBase::ResetItemFlags() {
	bIsCopy = false;
	bIsPickup = false;
}

UItemBase* UItemBase::CreateItemCopy() const {
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());
	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemNumericData = this->ItemNumericData;
	ItemCopy->ItemStatisticsData = this->ItemStatisticsData;
	ItemCopy->ItemAssetData = this->ItemAssetData;
	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity) {
	if(NewQuantity != Quantity) {
		Quantity = FMath::Clamp(NewQuantity, 0, ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

		if(OwningInventory) {
			if(Quantity <= 0) {
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
	}
}

void UItemBase::Use(AMegaCharacter* Character) {}
