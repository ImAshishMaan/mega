#include "CharacterHealthWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCharacterHealthWidget::NativeConstruct() {
	Super::NativeConstruct();
	InitializeHealthMaterial();
}

void UCharacterHealthWidget::InitializeHealthMaterial() {
	if (HealthMaterialImage && HealthMaterialImage->GetBrush().GetResourceObject()) {
		// Create a dynamic material instance from the existing material
		UMaterialInterface* BaseMaterial =  Cast<UMaterialInterface>(HealthMaterialImage->GetBrush().GetResourceObject());
		DynamicHealthMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);

		if(DynamicHealthMaterial) {
			HealthMaterialImage->SetBrushFromMaterial(DynamicHealthMaterial);
			SetHealth(100, 100);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance for HealthMaterialImage"));
		}
	}
}

void UCharacterHealthWidget::SetHealth(float Health, float MaxHealth) {
	if (!DynamicHealthMaterial) {
		UE_LOG(LogTemp, Warning, TEXT("Dynamic material not initialized for Character Health Widget!"));
		return;
	}

	float HealthPercentage = (MaxHealth > 0.0f) ? FMath::Clamp(1.0f - (Health / MaxHealth), 0.0f, 1.0f) : 1.0f;

	DynamicHealthMaterial->SetScalarParameterValue(TEXT("Animate_HealthNewToUpdated"), HealthPercentage);

	if (HealthText) {
		HealthText->SetText(FText::AsNumber(FMath::CeilToInt(Health)));
	}
}

