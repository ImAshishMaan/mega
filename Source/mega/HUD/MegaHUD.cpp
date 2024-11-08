#include "MegaHUD.h"
#include "CharacterOverlayWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Texture2D.h"
#include "mega/UserInterface/MainMenu.h"
#include "mega/UserInterface/Interaction/InteractionWidget.h"

AMegaHUD::AMegaHUD() {
	
}

void AMegaHUD::BeginPlay() {
	Super::BeginPlay();
	
	if(PauseUIManager == nullptr) {
		PauseUIManager = NewObject<UPauseUIManager>(this, UPauseUIManager::StaticClass());
	}

	InitOverlays();
	AddCharacterOverlay();
}

void AMegaHUD::InitOverlays() {
	APlayerController* PlayerController = GetOwningPlayerController();
	if(PlayerController) {
		if(CharacterOverlayClass && CharacterOverlay == nullptr) {
			CharacterOverlay = CreateWidget<UCharacterOverlayWidget>(PlayerController, CharacterOverlayClass);
		}
	}

	if(MainMenuClass) {
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if(InteractionWidgetClass) {
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMegaHUD::DisplayMenu() {
	if(MainMenuWidget) {
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMegaHUD::HideMenu() {
	if(MainMenuWidget) {
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
void AMegaHUD::ShowInteractionWidget() {
	if(InteractionWidget) {
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMegaHUD::HideInteractionWidget() {
	if(InteractionWidget) {
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}
void AMegaHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) {
	if(InteractionWidget) {
		if(InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed) {
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AMegaHUD::AddCharacterOverlay() {
	if(CharacterOverlay) {
		CharacterOverlay->AddToViewport();
	}
}

void AMegaHUD::DrawHUD() {
	Super::DrawHUD();

	// TODO: Need to move this somewhere else
	FHUDPackage HUDPackageTemp;
	if(CrosshairsCenter) {
		HUDPackageTemp.CrosshairsCenter = CrosshairsCenter;
	}

	if(CrosshairsLeft) {
		HUDPackageTemp.CrosshairsLeft = CrosshairsLeft;
	}

	if(CrosshairsRight) {
		HUDPackageTemp.CrosshairsRight = CrosshairsRight;
	}

	if(CrosshairsTop) {
		HUDPackageTemp.CrosshairsTop = CrosshairsTop;
	}

	if(CrosshairsBottom) {
		HUDPackageTemp.CrosshairsBottom = CrosshairsBottom;
	}
	SetHUDPackage(HUDPackageTemp);


	FVector2D ViewportSize;
	if(GEngine) {
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		if(HUDPackage.CrosshairsCenter) {
			DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter);
		}

		if(HUDPackage.CrosshairsLeft) {
			DrawCrosshairs(HUDPackage.CrosshairsLeft, ViewportCenter);
		}

		if(HUDPackage.CrosshairsRight) {
			DrawCrosshairs(HUDPackage.CrosshairsRight, ViewportCenter);
		}

		if(HUDPackage.CrosshairsTop) {
			DrawCrosshairs(HUDPackage.CrosshairsTop, ViewportCenter);
		}

		if(HUDPackage.CrosshairsBottom) {
			DrawCrosshairs(HUDPackage.CrosshairsBottom, ViewportCenter);
		}
	}
}

void AMegaHUD::DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter) {
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(ViewportCenter.X - TextureWidth / 2, ViewportCenter.Y - TextureHeight / 2);

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		FColor::White
	);
}
