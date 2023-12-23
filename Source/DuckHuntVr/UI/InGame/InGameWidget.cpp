#include "InGameWidget.h"
#include "Panels/HitPanelWidget.h"
#include "Panels/ShotPanelWidget.h"

void UInGameWidget::NativePreConstruct() {
	Super::NativePreConstruct();
	SetTheme(Theme);
}

void UInGameWidget::SetTheme(const ETheme NewTheme) {
	Theme = NewTheme;
	if (ShotPanel)
		ShotPanel->SetTheme(NewTheme);
	if (HitPanel)
		HitPanel->SetTheme(NewTheme);
}
