#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UInventoryComponent;
class UItemBase;
/**
 * 
 */
UCLASS()
class MEGA_API UItemDragDropOperation : public UDragDropOperation {
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;
};
