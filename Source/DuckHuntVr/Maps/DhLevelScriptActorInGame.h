#pragma once

#include "DhLevelScriptActorBase.h"
#include "DhLevelScriptActorInGame.generated.h"

class FOnMovieSceneSequencePlayerEvent;
class AInGameUI;
class ULevelSequence;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhLevelScriptActorInGame : public ADhLevelScriptActorBase {
	GENERATED_BODY()
public:
#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE void PlayStartGameSequence(const FOnMovieSceneSequencePlayerEvent& OnFinishedEvent) const {
		PlayLevelSequence(StartGameSequence.Get(), OnFinishedEvent);
	}

	FORCEINLINE void PlayStartRoundSequence(const FOnMovieSceneSequencePlayerEvent& OnFinishedEvent) const {
		if (StartRoundSequence)
			PlayLevelSequence(StartRoundSequence.Get(), OnFinishedEvent);
	}

	void PlayPauseSound() const;

	FORCEINLINE void SetDefaultSkyColor() { SetSkyColor(DefaultSkyColor); }
	FORCEINLINE void SetFlyAwaySkyColor() { SetSkyColor(FlyAwaySkyColor); }

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void OpenMainMenuLevel();

	FORCEINLINE AVrPawn* GetPawn() const { return Pawn.Get(); }
	FORCEINLINE UDhGameInstance* GetDhGameInstance() const { return GameInstance.Get(); }

	FORCEINLINE const AInGameUI* GetInGameUI() const { return InGameUI.Get(); }
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
	TObjectPtr<ULevelSequence> StartGameSequence;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<ULevelSequence> StartRoundSequence;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> PauseSound;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AInGameUI> InGameUI;
private:
	void PlayLevelSequence(ULevelSequence* Sequence, const FOnMovieSceneSequencePlayerEvent& OnFinishedEvent) const;
};
