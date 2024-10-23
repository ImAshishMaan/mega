#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class MEGA_API UMWorldUserWidget : public UUserWidget {
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* SizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;
};
