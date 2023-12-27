#include "DhGameStateBaseInGame.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"
#include "DuckHuntVr/Maps/DhLevelScriptActorInGame.h"
#include "DuckHuntVr/UI/InGame/InGameUIActor.h"
#include "Kismet/KismetSystemLibrary.h"

void ADhGameStateBaseInGame::BeginPlay() {
	LevelScriptActor = CastChecked<ADhLevelScriptActorInGame>(GetWorld()->GetLevelScriptActor());
	Super::BeginPlay();
}

void ADhGameStateBaseInGame::TogglePause() {
	Super::TogglePause();

	const auto GameMode = AuthorityGameMode.Get();
	const auto InGameUI = LevelScriptActor->GetInGameUI();

	if (!GameMode->IsPaused()) {
		InGameUI->SetStateInfoPause();
		InGameUI->Redraw();
		LevelScriptActor->PlayPauseSound();
		// TODO: hide ducks if any
		GameMode->SetPause(LevelScriptActor->GetPawn()->GetPlayerController());
	}
	else if (AllowedToUnpause) {
		InGameUI->ClearStateInfo();
		InGameUI->Redraw();
		LevelScriptActor->PlayPauseSound();
		// TODO: show ducks if any
		// TODO: run ClearPause (below) after 1 sec delay
		GameMode->ClearPause();
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
		if (!AuthorityGameMode->IsPaused()) {
			if (GameInstance->GetVisualizationTypeGameStartedWith() != NewType) {
				AllowedToUnpause = false;
				TogglePause();
			}
		}
		else if (GameInstance->GetVisualizationTypeGameStartedWith() == NewType)
			AllowedToUnpause = true;
	}
}
