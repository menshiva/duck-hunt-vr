#include "InGameWidget.h"
#include "Panels/HitPanelWidget.h"
#include "Panels/ShotPanelWidget.h"

void UInGameWidget::NativePreConstruct() {
	Super::NativePreConstruct();
	SetTheme(DefaultTheme);
}

void UInGameWidget::SetTheme(const ETheme NewTheme) const {
	if (ShotPanel)
		ShotPanel->SetTheme(NewTheme);
	if (HitPanel)
		HitPanel->SetTheme(NewTheme);
}

void UInGameWidget::SetBulletsNum(const int32 NewNum) const {
	if (ShotPanel)
		ShotPanel->SetBulletsNum(NewNum);
}
