#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "mega/Enums.h"
#include "MegaHUD.generated.h"

class UTexture2D;

UCLASS()
class MEGA_API AMegaHUD : public AHUD {
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	/*
	 * Crosshairs
	 * TODO: Need to move this somewhere else
	 */

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsCenter = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsLeft = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsRight = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsTop = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairsBottom = nullptr;

	/*
	 * 
	 */
	

private:
	FHUDPackage HUDPackage;

	void DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter);

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) {
		HUDPackage = Package;
	}
};
