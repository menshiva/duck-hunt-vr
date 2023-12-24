#include "DhLevelScriptActorInGame.h"
#include "DuckHuntVr/UI/InGame/InGameUIActor.h"

ADhLevelScriptActorInGame::ADhLevelScriptActorInGame() {
	PrimaryActorTick.bCanEverTick = true;
}

#if WITH_EDITOR
void ADhLevelScriptActorInGame::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	if (!InGameUI.IsExplicitlyNull()) {
		InGameUI->SetSkyColor(DefaultSkyColor);
		InGameUI->Redraw();
	}
	SetRealBpSkyNewColor(DefaultSkyColor);
}
#endif

void ADhLevelScriptActorInGame::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	// TODO: rotate ui
}
