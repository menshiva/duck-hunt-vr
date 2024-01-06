#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/Hand/VisualizationType.h"
#include "GameFramework/GameStateBase.h"
#include "DhGameState.generated.h"

class ADhLevelScriptActorInGame;

enum class State : uint8 {
	LevelSequence,
	Playing,
	PlayingFlyAway,
	Paused,
	Unpausing
};

UCLASS(NotBlueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhGameState : public AGameStateBase {
	GENERATED_BODY()
public:
	ADhGameState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void Restart();

	bool OnGunFired(const AActor* HitActor) const;
	void TogglePause();
	void OnVisualizationTypeChanged(EVisualizationType NewType);
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	bool FlyAwayEnabled = false;
private:
	void SwitchStateInternal(State NewState);

	void OnInit();

	UFUNCTION()
	void OnGameStarted();

	bool OnBulletShot(const void* TargetActor) const;

	void OnGamePaused(bool Pause);
	void OnGameUnpaused();

	TWeakObjectPtr<ADhLevelScriptActorInGame> LevelScriptActor;

	State CurrentState = State::LevelSequence;
	State PreviousState = State::LevelSequence;

	bool AllowedToUnpause = true;
	FTimerHandle UnpauseTimerHandle;
};
