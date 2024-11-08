#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "mega/Character/MegaCharacter.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API UMainMenu : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY()
	AMegaCharacter* PlayerCharacter;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
