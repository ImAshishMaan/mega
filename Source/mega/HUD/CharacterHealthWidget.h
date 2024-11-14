#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHealthWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class MEGA_API UCharacterHealthWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	
	UPROPERTY(meta = (BindWidget))
    UImage* HealthMaterialImage;

	void SetHealth(float Health, float MaxHealth);

protected:
	virtual void NativeConstruct() override;

protected:

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicHealthMaterial;

	// Initialize the dynamic material
	void InitializeHealthMaterial();

};
