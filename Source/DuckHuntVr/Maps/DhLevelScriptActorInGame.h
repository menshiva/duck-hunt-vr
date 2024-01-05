#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorInGame.generated.h"

class AInGameUI;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhLevelScriptActorInGame : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	ADhLevelScriptActorInGame();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void PlayPauseSound() const;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSkyColor(const FLinearColor& Color);
	void SetDefaultSkyColor() { SetSkyColor(DefaultSkyColor); }

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenMainMenuLevel();

	FORCEINLINE const AInGameUI* GetInGameUI() const { return InGameUI.Get(); }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<ALevelSequenceActor> StartGameSequence;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	FLinearColor DefaultSkyColor;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> PauseSound;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AInGameUI> InGameUI;
private:
	UFUNCTION()
	void OnStartGameSequenceEnd();
};
