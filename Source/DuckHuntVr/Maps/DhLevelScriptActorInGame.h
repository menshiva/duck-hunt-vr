#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorInGame.generated.h"

class AInGameUI;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ADhLevelScriptActorInGame : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	ADhLevelScriptActorInGame();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSkyColor(const FLinearColor& Color);

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenMainMenuLevel() { OpenLevel(MainMenuLevel); }

	FORCEINLINE const FLinearColor& GetDefaultSkyColor() const { return DefaultSkyColor; }
	FORCEINLINE const AInGameUI* GetInGameUI() const { return InGameUI.Get(); }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	FLinearColor DefaultSkyColor;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AInGameUI> InGameUI;
};
