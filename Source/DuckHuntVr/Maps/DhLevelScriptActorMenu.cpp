#include "DhLevelScriptActorMenu.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"

void ADhLevelScriptActorMenu::OpenDuckLevel() {
	GameInstance->SetVisualizationTypeGameStartedWith(Pawn->GetVisualizationType());
	OpenLevel(DuckLevel);
}

void ADhLevelScriptActorMenu::OpenClayLevel() {
	GameInstance->SetVisualizationTypeGameStartedWith(Pawn->GetVisualizationType());
	OpenLevel(ClayLevel);
}
