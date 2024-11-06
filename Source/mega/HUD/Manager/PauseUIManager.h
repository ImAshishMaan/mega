#pragma once

#include "CoreMinimal.h"
#include "PauseUIManager.generated.h"

/**
 * 
 */
UCLASS()
class MEGA_API UPauseUIManager : public UObject {
	GENERATED_BODY()

public:

	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PlayerController);

	void PopWidget(APlayerController* PlayerController);

private:

	// Stack to keep track of the widgets
	TArray<UUserWidget*> WidgetStack;

	//TODO: Use Widget Cache so we don't instantiate the same widget multiple times
};
