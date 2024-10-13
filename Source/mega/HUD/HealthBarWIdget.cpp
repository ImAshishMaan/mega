#include "HealthBarWIdget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"
#include "mega/MegaComponents/AttributeComponent.h"

void UHealthBarWIdget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
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

void UHealthBarWIdget::NativeConstruct() {
	Super::NativeConstruct();

	UAttributeComponent* AttributeComp = UAttributeComponent::GetAttributes(AttachedActor);
	if(AttributeComp) {
		AttributeComp->OnHealthChanged.AddDynamic(this, &UHealthBarWIdget::UpdateHealth);
	}
}

void UHealthBarWIdget::UpdateHealth(AActor* InstigatorActor, UAttributeComponent* OwningComp, float NewHealth, float MaxHealth, float Damage) {
	HealthBar->SetPercent(NewHealth/MaxHealth);

	if(NewHealth <= 0.0f) {
		RemoveFromParent();
	}
}
