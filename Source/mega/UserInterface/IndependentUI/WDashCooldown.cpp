#include "WDashCooldown.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UWDashCooldown::NativeConstruct() {
	Super::NativeConstruct();

	ElapsedTime = 0.0f;

	if(BarFill) {
		UMaterialInterface* Material = Cast<UMaterialInterface>(BarFill->GetBrush().GetResourceObject());
		if(Material) {
			UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
			if(MaterialInstance) {
				BarFill->SetBrushFromMaterial(MaterialInstance);
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(FillTimerHandle, this, &UWDashCooldown::UpdateArcAnimateFill, 0.01f, true);
}

void UWDashCooldown::UpdateArcAnimateFill() {
	if(BarFill) {
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(BarFill->GetBrush().GetResourceObject());
		if(MaterialInstance) {
			ElapsedTime += 0.01f;

			float FillValue = FMath::Clamp(ElapsedTime / CoolDownTime, 0.0f, 1.0f);
			MaterialInstance->SetScalarParameterValue(FName("AnimateArcFill"), FillValue);

			if(FillValue >= 1.0f) {
				GetWorld()->GetTimerManager().ClearTimer(FillTimerHandle);
			}
		}
	}
}
