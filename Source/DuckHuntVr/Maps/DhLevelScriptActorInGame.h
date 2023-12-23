#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorInGame.generated.h"

UCLASS(Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ADhLevelScriptActorInGame : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenMainMenu() { OpenLevel(MainMenuLevel); }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> MainMenuLevel;
};
