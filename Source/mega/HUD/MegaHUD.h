#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Manager/PauseUIManager.h"
#include "mega/Enums.h"
#include "MegaHUD.generated.h"

class UInteractionWidget;
class UMainMenu;
struct FInteractableData;
class UCharacterOverlayWidget;
class UTexture2D;
class UUserWidget;

UCLASS()
class MEGA_API AMegaHUD : public AHUD {
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	void InitOverlays();

	/*
	 * UI manager reference
	 */
	UPROPERTY()
	UPauseUIManager* PauseUIManager;

	UPROPERTY(EditAnywhere, Category = "Overlays")
	TSubclassOf<UUserWidget> CharacterOverlayClass;

	UPROPERTY()
	UCharacterOverlayWidget* CharacterOverlay;

	/*
	 * UI setttings
	 */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SettingMenuClass;

	/*
	 * Add Overlays to Viewport
	 */

	void AddCharacterOverlay();
	

	/*
	 * Default Cross-hairs
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
	 *  Inventory 
	 */
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;
	
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;


	AMegaHUD();

	void DisplayMenu();
	void HideMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

private:
	FHUDPackage HUDPackage;

	void DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter);

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) {
		HUDPackage = Package;
	}
};
