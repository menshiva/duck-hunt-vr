#include "ShotPanelWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

void UShotPanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetSkyColor(SkyColor);
	ResetBullets();
}

void UShotPanelWidget::SetSkyColor(const FLinearColor& NewColor) {
	SkyColor = NewColor;
	if (BackgroundSky)
		BackgroundSky->SetColorAndOpacity(NewColor);
}

bool UShotPanelWidget::RemoveBullet() {
	if (BulletsNum) {
		--BulletsNum;
		const auto& Slots = ImagesBox->GetSlots();
		if (BulletsNum < Slots.Num())
			Slots[BulletsNum]->Content->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		return true;
	}
	return false;
}

void UShotPanelWidget::ResetBullets() {
	BulletsNum = DefaultBulletsNum;
	check(0 <= BulletsNum && BulletsNum < 4);
	if (ImagesBox) {
		const auto& Slots = ImagesBox->GetSlots();
		if (BulletsNum <= Slots.Num()) {
			for (int32 i = 0; i < BulletsNum; ++i)
				Slots[i]->Content->SetVisibility(ESlateVisibility::Hidden);

			for (int32 i = BulletsNum; i < Slots.Num(); ++i)
				Slots[i]->Content->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
