#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "AssetLoaderUtility.generated.h"

UCLASS()
class MEGA_API UAssetLoaderUtility : public UObject {
	GENERATED_BODY()

public:
	// Cache for preloaded assets
	static TMap<FString, UClass*> CachedClasses;
	static TMap<FString, UObject*> CachedObjects;

	// Preload and cache a class
	template <typename T>
	static void PreloadClass(TSoftClassPtr<T> AssetToPreload) {
		// Check if already cached
		if(CachedClasses.Contains(AssetToPreload.ToString())) {
			return; // Already cached
		}

		// Asynchronously load the class and cache it
		GetStreamableManager().RequestAsyncLoad(AssetToPreload.ToSoftObjectPath(), [AssetToPreload]() {
			UClass* LoadedClass = AssetToPreload.Get();
			if(LoadedClass) {
				CachedClasses.Add(AssetToPreload.ToString(), LoadedClass);
			} else {
				UE_LOG(LogTemp, Error, TEXT("Failed to preload class: %s"), *AssetToPreload.ToString());
			}
		});
	}

	template <typename T>
	static void PreloadObject(TSoftObjectPtr<T> AssetToPreload) {
		if(CachedObjects.Contains(AssetToPreload.ToString())) {
			return;
		}

		GetStreamableManager().RequestAsyncLoad(AssetToPreload.ToSoftObjectPath(), [AssetToPreload]() {
			T* LoadedObject = AssetToPreload.Get();
			if(LoadedObject) {
				CachedObjects.Add(AssetToPreload.ToString(), LoadedObject);
			} else {
				UE_LOG(LogTemp, Error, TEXT("Failed to preload object: %s"), *AssetToPreload.ToString());
			}
		});
	}

	template <typename T>
	static void LoadClassAsync(TSoftClassPtr<T> AssetToLoad, TFunction<void(UClass*)> OnAssetLoaded = nullptr) {
		if(CachedClasses.Contains(AssetToLoad.ToString())) {
			if(OnAssetLoaded) {
				OnAssetLoaded(CachedClasses[AssetToLoad.ToString()]);
			}
			return;
		}

		GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(), [AssetToLoad, OnAssetLoaded]() {
			UClass* LoadedClass = AssetToLoad.Get();
			if(LoadedClass) {
				CachedClasses.Add(AssetToLoad.ToString(), LoadedClass);
				if(OnAssetLoaded) {
					OnAssetLoaded(LoadedClass);
				}
			} else {
				UE_LOG(LogTemp, Error, TEXT("Failed to load class: %s"), *AssetToLoad.ToString());
			}
		});
	}

	template <typename T>
	static void LoadObjectAsync(TSoftObjectPtr<T> AssetToLoad, TFunction<void(UObject*)> OnAssetLoaded = nullptr) {
		if(CachedObjects.Contains(AssetToLoad.ToString())) {
			if(OnAssetLoaded) {
				OnAssetLoaded(CachedObjects[AssetToLoad.ToString()]);
			}
			return;
		}

		GetStreamableManager().RequestAsyncLoad(AssetToLoad.ToSoftObjectPath(), [AssetToLoad, OnAssetLoaded]() {
			T* LoadedObject = AssetToLoad.Get();
			if(LoadedObject) {
				CachedObjects.Add(AssetToLoad.ToString(), LoadedObject);
				if(OnAssetLoaded) {
					OnAssetLoaded(LoadedObject);
				}
			} else {
				UE_LOG(LogTemp, Error, TEXT("Failed to load object: %s"), *AssetToLoad.ToString());
			}
		});
	}

	template <typename T>
	static void UnloadAsset(TSoftObjectPtr<T>& SoftPointer) {
		if(SoftPointer.IsValid()) {
			GetStreamableManager().Unload(SoftPointer.ToSoftObjectPath());
			SoftPointer.Reset();
		}
	}

	static void ClearAllCachedAssets(bool ClassesAssets, bool ObjectsAssets) {
		if(ClassesAssets) {
			for(auto Pair: CachedClasses) {
				GetStreamableManager().Unload(Pair.Value->GetPathName());
			}
			CachedClasses.Empty();
		}
		if(ObjectsAssets) {
			for(auto Pair: CachedObjects) {
				GetStreamableManager().Unload(Pair.Value->GetPathName());
			}
			CachedObjects.Empty();
		}
	}

private:
	// Helper to get the Streamable Manager from Asset Manager
	static FStreamableManager& GetStreamableManager() {
		return UAssetManager::GetStreamableManager();
	}
};
