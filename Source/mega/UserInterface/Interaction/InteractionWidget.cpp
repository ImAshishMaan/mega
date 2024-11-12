#include "InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "mega/Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgressBar");
}

void UInteractionWidget::NativeConstruct() {
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) {
	switch(InteractableData->InteractionType) {
	case EInteractionType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if(InteractableData->Quantity <= 1) {
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		} else {
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteracctionWidget", "QuantityText", "x{0}"), FText::AsNumber(InteractableData->Quantity)));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		
		break;
	case EInteractionType::NonPlayerCharacter:
		break;
	case EInteractionType::Device:
		break;
	case EInteractionType::Toggle:
		break;
	case EInteractionType::Container:
		break;

	default: ;
	}
	ActionText->SetText(FText::FromString(InteractableData->Action.ToString()));
	NameText->SetText(InteractableData->Name);
}

float UInteractionWidget::UpdateInteractionProgressBar() {
	return 0.0f;
}

