#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "AssetLoaderUtility.generated.h"


UCLASS()
class MEGA_API UAssetLoaderUtility : public UObject {
	GENERATED_BODY()

public:

	template <typename T>
	static void LoadAssetAsync(TSoftClassPtr<T> AssetToLoad, TFunction<void(UClass*)> OnAssetLoaded = nullptr) {
		if(AssetToLoad.IsValid()) {
			if(OnAssetLoaded) {
				OnAssetLoaded(AssetToLoad.Get());
			}
		} else {
			GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(), [AssetToLoad, OnAssetLoaded]() {
				if(OnAssetLoaded) {
					OnAssetLoaded(AssetToLoad.Get());
				} else {
					// Error handling: Asset failed to load
					UE_LOG(LogTemp, Error, TEXT("Failed to load class: %s"), *AssetToLoad.ToString());
				}
			});
		}
	}

	template <typename T>
	static void LoadAssetAsync(TSoftObjectPtr<T> AssetToLoad, TFunction<void(UClass*)> OnAssetLoaded = nullptr) {
		if(AssetToLoad.IsValid()) {
			if(OnAssetLoaded) {
				OnAssetLoaded(AssetToLoad.Get());
			}
		} else {
			GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(), [AssetToLoad, OnAssetLoaded]() {
				if(OnAssetLoaded) {
					OnAssetLoaded(AssetToLoad.Get());
				} else {
					// Error handling: Asset failed to load
					UE_LOG(LogTemp, Error, TEXT("Failed to load class: %s"), *AssetToLoad.ToString());
				}
			});
		}
	}

private:
	// Helper to get the Streamable Manager from Asset Manager
	static FStreamableManager& GetStreamableManager() {
		return UAssetManager::GetStreamableManager();
	}
};
