#include "PauseUIManager.h"
#include "Blueprint/UserWidget.h"

void UPauseUIManager::ShowWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PlayerController) {
	if(WidgetClass && PlayerController) {
		for (UUserWidget* Widget : WidgetStack) {
			if (Widget->GetClass() == WidgetClass) {
				Widget->AddToViewport();
				return;
			}
		}
		
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
		if(NewWidget) {
			if(WidgetStack.Num() > 0) {
				WidgetStack.Last()->RemoveFromParent();
			}

			NewWidget->AddToViewport();
			WidgetStack.Push(NewWidget);
		}
	}
}

void UPauseUIManager::PopWidget(APlayerController* PlayerController) {
	if(WidgetStack.Num() > 0) {
		UUserWidget* TopWidget = WidgetStack.Pop();
		if(TopWidget) {
			TopWidget->RemoveFromParent();
		}

		if(WidgetStack.Num() > 0) {
			WidgetStack.Last()->AddToViewport();
		}
	}
}
