#pragma once

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),

	EWS_Max UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	EWT_Rifle UMETA(DisplayName = "Rifle"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),

	EWT_Max UMETA(DisplayName = "DefaultMAX")
};