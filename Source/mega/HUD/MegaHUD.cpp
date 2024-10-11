#include "MegaHUD.h"
#include "CharacterOverlayWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

void AMegaHUD::BeginPlay() {
	Super::BeginPlay();

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
