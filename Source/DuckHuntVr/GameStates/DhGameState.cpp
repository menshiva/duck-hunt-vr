#include "DhGameState.h"
#include "MovieSceneSequencePlayer.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"
#include "DuckHuntVr/Maps/DhLevelScriptActorInGame.h"
#include "DuckHuntVr/UI/InGame/InGameUIActor.h"
#include "Kismet/KismetSystemLibrary.h"

ADhGameState::ADhGameState() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ADhGameState::BeginPlay() {
	LevelScriptActor = CastChecked<ADhLevelScriptActorInGame>(GetWorld()->GetLevelScriptActor());
	Super::BeginPlay();
	OnInit();
}

void ADhGameState::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	// Fixes timer when game is unpausing
	if (CurrentState == State::Unpausing)
		GetWorldTimerManager().Tick(DeltaSeconds);
}

void ADhGameState::Restart() {
	const auto CurrentVisType = LevelScriptActor->GetPawn()->GetVisualizationType();
	if (CurrentVisType != EVisualizationType::None)
		LevelScriptActor->GetDhGameInstance()->SetGameVisualizationType(CurrentVisType);
	OnInit();
}

bool ADhGameState::OnGunFired(const AActor* HitActor) const {
	switch (CurrentState) {
		case State::LevelSequence:
			return !LevelScriptActor->GetDhGameInstance()->HasGameMapOpenedFromMenu(); // TODO: return false;
		case State::Playing:
		case State::PlayingFlyAway:
			return OnBulletShot(HitActor);
		case State::Paused:
			return true;
		case State::Unpausing:
			return false;
		default:
			return false;
	}
}

void ADhGameState::TogglePause() {
	if (CurrentState != State::Paused) {
		OnGamePaused(true);
	}
	else if (AllowedToUnpause) {
		OnGamePaused(false);
	}
	else {
		// TODO: create notification
		UKismetSystemLibrary::PrintString(
			this, TEXT("Please return to the controller type you started the game with or restart the game."),
			true, true, FLinearColor::Green
		);
	}
}

void ADhGameState::OnVisualizationTypeChanged(const EVisualizationType NewType) {
	if (NewType == EVisualizationType::None)
		return;

	// TODO: remove
	const auto GameInstance = LevelScriptActor->GetDhGameInstance();
	if (!GameInstance->HasGameMapOpenedFromMenu())
		return;

	if (CurrentState != State::Paused) {
		if (GameInstance->GetGameVisualizationType() != NewType) {
			AllowedToUnpause = false;
			OnGamePaused(true);
		}
	}
	else
		AllowedToUnpause = GameInstance->GetGameVisualizationType() == NewType;
}

void ADhGameState::SwitchStateInternal(const State NewState) {
	PreviousState = CurrentState;
	CurrentState = NewState;
}

void ADhGameState::OnInit() {
	CurrentState = PreviousState = State::LevelSequence;
	AllowedToUnpause = true;
	GetWorldTimerManager().ClearTimer(UnpauseTimerHandle);
	if (AuthorityGameMode->IsPaused())
		AuthorityGameMode->ClearPause();

	LevelScriptActor->SetDefaultSkyColor();
	LevelScriptActor->GetInGameUI()->ClearStateInfo();

	// TODO: reset ducks (clays)

	FOnMovieSceneSequencePlayerEvent OnStartGameSequenceFinished;
	OnStartGameSequenceFinished.AddDynamic(this, &ADhGameState::OnGameStarted);
	LevelScriptActor->PlayStartGameSequence(OnStartGameSequenceFinished);
}

void ADhGameState::OnGameStarted() {
	SwitchStateInternal(State::Playing);
	// TODO: let the ducks (clays) out
}

bool ADhGameState::OnBulletShot(const void* TargetActor) const {
	const auto InGameUI = LevelScriptActor->GetInGameUI();
	if (InGameUI->RemoveBullet()) {
		if (TargetActor) {
			// TODO: kill target
		}
		return true;
	}

	// TODO: remove
	if (!LevelScriptActor->GetDhGameInstance()->HasGameMapOpenedFromMenu())
		InGameUI->ResetBullets();

	return false;
}

void ADhGameState::OnGamePaused(const bool Pause) {
	if (Pause) {
		GetWorldTimerManager().ClearTimer(UnpauseTimerHandle);
		SwitchStateInternal(State::Paused);

		// TODO: hide ducks if any

		LevelScriptActor->PlayPauseSound();
		LevelScriptActor->GetInGameUI()->SetStateInfoPause();

		AuthorityGameMode->SetPause(LevelScriptActor->GetPawn()->GetPlayerController());
	}
	else {
		SwitchStateInternal(PreviousState); // so that we can set to previous state after unpausing
		SwitchStateInternal(State::Unpausing);

		// TODO: show ducks if any

		LevelScriptActor->PlayPauseSound();
		LevelScriptActor->GetInGameUI()->ClearStateInfo();

		GetWorldTimerManager().SetTimer(UnpauseTimerHandle, this, &ADhGameState::OnGameUnpaused, 1.f, false);
	}
}

void ADhGameState::OnGameUnpaused() {
	SwitchStateInternal(PreviousState);
	AuthorityGameMode->ClearPause();
}
