#include "AssetLoaderUtility.h"

// Define the static member variables
TMap<FString, UClass*> UAssetLoaderUtility::CachedClasses;
TMap<FString, UObject*> UAssetLoaderUtility::CachedObjects;
