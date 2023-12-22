#include "DhGameModeBase.h"

ADhGameModeBase::ADhGameModeBase() {
	GameSessionClass = nullptr;
	HUDClass = nullptr;
}

bool ADhGameModeBase::AllowPausing(APlayerController* PC) {
	return bPauseable /*|| GetNetMode() == NM_Standalone*/;
}
