#include "InventoryComponent.h"

#include "mega/Items/ItemBase.h"

UInventoryComponent::UInventoryComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay() {
	Super::BeginPlay();
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const {
	if(ItemIn) {
		if(InventoryContents.Contains(ItemIn)) {
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const {
	if(ItemIn) {
		if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn)) {
			// TODO: Look into this again little confusing. (double pointer) 
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const {
	if(const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem) {
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
		}
	)) {
		return *Result;
	}
	return nullptr;
}

int32 UInventoryComponent::CalculateWeightAddAmount(UItemBase* InputIn, int32 RequestedAddAmount) {
	const int32 WeightMaxAddAmount = FMath::FloorToInt(GetWeightCapacity() - InventoryTotalWeight / InputIn->GetItemSingleWeight());
	if(WeightMaxAddAmount >= RequestedAddAmount) {
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount) {
	const int32 AddAmountToMakeFullStack = StackableItem->ItemNumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove) {
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove) {
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);
	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->GetItemSingleWeight();
	OnInventoryUpdated.Broadcast();
	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn, int32 AmountToSplit) {
	if(!(InventoryContents.Num() + 1 > InventorySlotsCapacity)) {
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* InputItem) {
	// check is input item has valid weight
	if(FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0) {
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. Invalid weight."), InputItem->ItemTextData.Name));
	}

	// will the item weight overflow weight capacity
	if(InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity()) {
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. Item would overflow weight capacity."), InputItem->ItemTextData.Name));
	}

	// adding one more item would overflow the slot capacity
	if(InventoryContents.Num() + 1 > InventorySlotsCapacity) {
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. All inventory slots are full."), InputItem->ItemTextData.Name));
	}

	AddNewItem(InputItem, 1);
	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("Successfully added {0} {1} to inventory."), 1, InputItem->ItemTextData.Name));
	
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* InputItem, int32 RequestedAddAmount) {
	return 0; 
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem) {
	if(GetOwner()) {
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// Handle non-stackable items
		if(!InputItem->ItemNumericData.bIsStackable) {
			return HandleNonStackableItems(InputItem);
		}

		// Handle stackable items
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

		if(StackableAmountAdded == InitialRequestedAddAmount) {
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(FText::FromString("Successfully added {0} {1} to inventory."), InitialRequestedAddAmount, InputItem->ItemTextData.Name));
		}
		if(StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0) {
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(FText::FromString("Partial amount of {0} added to inventory. Number added = {1}."), InputItem->ItemTextData.Name, StackableAmountAdded));
		}
		if(StackableAmountAdded <= 0) {
			return FItemAddResult::AddedNone(FText::Format(FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."), InputItem->ItemTextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. Unknown error."), InputItem->ItemTextData.Name));
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd) {
	UItemBase* NewItem;
	if(Item->bIsCopy || Item->bIsPickup || true) { // TODO: FIX:: Data is getting lost here if we go to else state
		// if item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	} else {
		// used when splitting or dragging to/from inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}
