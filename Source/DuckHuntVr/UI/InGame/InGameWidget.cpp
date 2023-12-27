#include "InGameWidget.h"
#include "Components/Image.h"

void UInGameWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetStateInfo(StateInfo);
}

void UInGameWidget::SetStateInfo(const EStateInfo NewStateInfo) {
	StateInfo = NewStateInfo;
	if (StateInfoImage) {
		switch (StateInfo) {
			case EStateInfo::None:
				StateInfoImage->SetVisibility(ESlateVisibility::Collapsed);
				break;
			case EStateInfo::FlyAway:
				StateInfoImage->SetBrushFromTexture(FlyAwayStateTexture, true);
				StateInfoImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				break;
			case EStateInfo::Pause:
				StateInfoImage->SetBrushFromTexture(PauseStateTexture, true);
				StateInfoImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				break;
		}
	}
}
