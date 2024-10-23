#include "MWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UMWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(AttachedActor)) {
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("Attached Actor no longer valid, removing Health Widget."));
		return;
	}

	FVector2D ScreenPosition;
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition)) {
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		if(SizeBox) {
			SizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
	
}
