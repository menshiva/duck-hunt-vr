#include "ShotPanelWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UShotPanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetSkyColor(SkyColor);
	SetBulletsNum(BulletsNum);
}

void UShotPanelWidget::SetSkyColor(const FLinearColor& NewColor) {
	SkyColor = NewColor;
	if (BackgroundSky)
		BackgroundSky->SetColorAndOpacity(NewColor);
}

void UShotPanelWidget::SetBulletsNum(const int32 NewNum) {
	check(0 <= NewNum && NewNum < 4);
	BulletsNum = NewNum;

	if (ImagesBox) {
		const auto& Slots = ImagesBox->GetSlots();
		if (NewNum <= Slots.Num()) {
			for (int32 i = 0; i < NewNum; ++i)
				Slots[i]->Content->SetVisibility(ESlateVisibility::Hidden);

			for (int32 i = NewNum; i < Slots.Num(); ++i)
				Slots[i]->Content->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
