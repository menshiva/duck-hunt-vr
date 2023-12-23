#include "DhGameModeBase.h"

ADhGameModeBase::ADhGameModeBase() {
	GameSessionClass = nullptr;
	HUDClass = nullptr;
}

bool ADhGameModeBase::AllowPausing(APlayerController* PC) {
	return bPauseable /*|| GetNetMode() == NM_Standalone*/;
}

void ADhGameModeBase::SetPause(APlayerController* PC, const bool Pause) {
	if (Pause)
		Super::SetPause(PC);
	else
		Super::ClearPause();
}
