#include "DhLevelScriptActorMenu.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"

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
