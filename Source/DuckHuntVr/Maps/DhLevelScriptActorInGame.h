#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorInGame.generated.h"

class AInGameUI;

UCLASS(Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ADhLevelScriptActorInGame : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	ADhLevelScriptActorInGame();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	virtual void Tick(float DeltaSeconds) override;

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenMainMenu() { OpenLevel(MainMenuLevel); }
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetRealBpSkyNewColor(const FLinearColor& NewColor);

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	FLinearColor DefaultSkyColor;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AInGameUI> InGameUI;
};
