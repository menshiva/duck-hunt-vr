#include "DhGameStateBaseInGame.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"
#include "DuckHuntVr/Maps/DhLevelScriptActorInGame.h"
#include "DuckHuntVr/UI/InGame/InGameUIActor.h"
#include "Kismet/KismetSystemLibrary.h"

ADhGameStateBaseInGame::ADhGameStateBaseInGame() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ADhGameStateBaseInGame::BeginPlay() {
	LevelScriptActor = CastChecked<ADhLevelScriptActorInGame>(GetWorld()->GetLevelScriptActor());
	Super::BeginPlay();
}

void ADhGameStateBaseInGame::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	// Fixes timer when game is paused
	if (GetWorldTimerManager().TimerExists(UnpauseTimerHandle))
		GetWorldTimerManager().Tick(DeltaSeconds);
}

void ADhGameStateBaseInGame::TogglePause() {
	Super::TogglePause();
	const auto InGameUI = LevelScriptActor->GetInGameUI();

	if (!IsPaused) {
		GetWorldTimerManager().ClearTimer(UnpauseTimerHandle);
		InGameUI->SetStateInfoPause();
		InGameUI->Redraw();
		LevelScriptActor->PlayPauseSound();
		// TODO: hide ducks if any
		IsPaused = true;
		AuthorityGameMode->SetPause(LevelScriptActor->GetPawn()->GetPlayerController());
	}
	else if (AllowedToUnpause) {
		InGameUI->ClearStateInfo();
		InGameUI->Redraw();
		LevelScriptActor->PlayPauseSound();
		// TODO: show ducks if any
		IsPaused = false;
		GetWorldTimerManager().SetTimer(UnpauseTimerHandle, this, &ADhGameStateBaseInGame::ClearPause, 1.f, false);
	}
	else {
		// TODO: create notification
		UKismetSystemLibrary::PrintString(
			this, TEXT("Please return to the controller type you started the game with or restart the game."),
			true, true, FLinearColor::Green
		);
	}
}

void ADhGameStateBaseInGame::OnVisualizationTypeChanged(const EVisualizationType NewType) {
	Super::OnVisualizationTypeChanged(NewType);
	const auto GameInstance = LevelScriptActor->GetDhGameInstance();
	if (GameInstance->GetVisualizationTypeGameStartedWith() != EVisualizationType::None && NewType != EVisualizationType::None) {
		if (!IsPaused) {
			if (GameInstance->GetVisualizationTypeGameStartedWith() != NewType) {
				AllowedToUnpause = false;
				TogglePause();
			}
		}
		else if (GameInstance->GetVisualizationTypeGameStartedWith() == NewType)
			AllowedToUnpause = true;
	}
}

void ADhGameStateBaseInGame::ClearPause() const {
	AuthorityGameMode->ClearPause();
}
