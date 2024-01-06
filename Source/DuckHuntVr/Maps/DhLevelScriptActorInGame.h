#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorInGame.generated.h"

class FOnMovieSceneSequencePlayerEvent;
class AInGameUI;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhLevelScriptActorInGame : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
	ADhLevelScriptActorInGame();

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	virtual void Tick(float DeltaSeconds) override;

	void PlayStartGameSequence(const FOnMovieSceneSequencePlayerEvent& OnFinishedEvent) const;
	void PlayPauseSound() const;
	FORCEINLINE void SetDefaultSkyColor() { SetSkyColor(DefaultSkyColor); }
	FORCEINLINE void SetFlyAwaySkyColor() { SetSkyColor(FlyAwaySkyColor); }

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenMainMenuLevel();

	FORCEINLINE const AInGameUI* GetInGameUI() const { return InGameUI.Get(); }

	FORCEINLINE AVrPawn* GetPawn() const { return Pawn.Get(); }
	FORCEINLINE UDhGameInstance* GetDhGameInstance() const { return GameInstance.Get(); }
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetSkyColor(const FLinearColor& Color);

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	FLinearColor DefaultSkyColor;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	bool FlyAwayEnabled = false;

	UPROPERTY(EditDefaultsOnly, Category=Init, meta=(EditCondition="FlyAwayEnabled"))
	FLinearColor FlyAwaySkyColor;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> PauseSound;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<ALevelSequenceActor> StartGameSequence;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AInGameUI> InGameUI;
};
