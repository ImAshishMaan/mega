#pragma once

#include "CoreMinimal.h"
#include "mega/MegaComponents/ActionComponent.h"
#include "MAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MEGA_API UMAction : public UObject {
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	bool bIsRunning;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	bool bAutoStart;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Actions")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Actions")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	FName ActionName;

	UWorld* GetWorld() const override;

};
