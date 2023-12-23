#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorMenu.generated.h"

UCLASS(Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ADhLevelScriptActorMenu : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenDuckLevel() { OpenLevel(DuckLevel); }

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenClayLevel() { OpenLevel(ClayLevel); }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> DuckLevel;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> ClayLevel;
};
