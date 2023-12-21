#include "InGameWidget.h"

void UInGameWidget::NativePreConstruct() {
	Super::NativePreConstruct();
	SetTheme(DefaultTheme);
}

void UInGameWidget::SetTheme(const ETheme NewTheme) const {
	if (ShotPanel)
		ShotPanel->SetTheme(NewTheme);
}
