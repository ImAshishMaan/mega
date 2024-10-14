#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

UENUM(BlueprintType)
enum class EEquipped : uint8 {
	UnEquipped = 0,
	Rifle = 1,
	Pistol = 2,

	MaxDefault = 3
};

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	Walking,
	Jogging,
	Crouching,
};

UENUM(BlueprintType)
enum class ELocomotionDirections : uint8 {
	Forward,
	Backward,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8 {
	Accumulate,
	BlendOut,
	Hold
};

USTRUCT(BlueprintType)
struct FCharacterSettings {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Settings")
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Settings")
	float MaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Settings")
	float BrakingDeceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Settings")
	float BrakingFrictionFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Settings")
	float BrakingFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Settings")
	bool bUseSeparateBrakingFriction;

	// Default constructor to initialize default values
	FCharacterSettings()
		: MaxWalkSpeed(800.f),
		  MaxAcceleration(800.f),
		  BrakingDeceleration(1200.f),
		  BrakingFrictionFactor(1.0f),
		  BrakingFriction(0.0f),
		  bUseSeparateBrakingFriction(true) {}

	FCharacterSettings(float InMaxWalkSpeed, float InMaxAcceleration, float InBrakingDeceleration, 
					   float InBrakingFrictionFactor, float InBrakingFriction, bool bInUseSeparateBrakingFriction)
		: MaxWalkSpeed(InMaxWalkSpeed),
		  MaxAcceleration(InMaxAcceleration),
		  BrakingDeceleration(InBrakingDeceleration),
		  BrakingFrictionFactor(InBrakingFrictionFactor),
		  BrakingFriction(InBrakingFriction),
		  bUseSeparateBrakingFriction(bInUseSeparateBrakingFriction) {}

};

USTRUCT(BlueprintType)
struct FHUDPackage {
	GENERATED_BODY()

public:
	UTexture2D* CrosshairsCenter = nullptr;
	UTexture2D* CrosshairsLeft = nullptr;
	UTexture2D* CrosshairsRight	= nullptr;
	UTexture2D* CrosshairsTop = nullptr;
	UTexture2D* CrosshairsBottom = nullptr;
	
};
