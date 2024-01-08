#include "MenuPanelWidget.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "DuckHuntVr/GameStates/DhGameState.h"
#include "DuckHuntVr/Maps/DhLevelScriptActorInGame.h"
#include "DuckHuntVr/UI/Widgets/ButtonWidget.h"

void UMenuPanelWidget::NativeConstruct() {
	Super::NativeConstruct();

	const auto InGameLevelScript = Cast<ADhLevelScriptActorInGame>(GetWorld()->GetLevelScriptActor());
	const auto GameState = InGameLevelScript->GetPawn()->GetGameState();

	RestartButton->SetOnClickEvent([GameState] {
		GameState->Restart();
	});

	OrientationButton->SetOnClickEvent([] {
		AVrPawn::ResetOrientationAndPosition();
	});

	MainMenuButton->SetOnClickEvent([InGameLevelScript] {
		InGameLevelScript->OpenMainMenuLevel();
	});
}
