#include "DhLevelScriptActorMenu.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"

void ADhLevelScriptActorMenu::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	const bool NewHmdWorn = AVrPawn::IsHmdWorn();
	if (!IsHmdWorn && NewHmdWorn)
		IntroSequence->SequencePlayer->Play();
	IsHmdWorn = NewHmdWorn;
}

void ADhLevelScriptActorMenu::OpenDuckLevel() {
	GameInstance->SetGameMapOpenedFromMenu(true);
	GameInstance->SetGameVisualizationType(Pawn->GetVisualizationType());
	OpenLevel(DuckLevel);
}

void ADhLevelScriptActorMenu::OpenClayLevel() {
	GameInstance->SetGameMapOpenedFromMenu(true);
	GameInstance->SetGameVisualizationType(Pawn->GetVisualizationType());
	OpenLevel(ClayLevel);
}
