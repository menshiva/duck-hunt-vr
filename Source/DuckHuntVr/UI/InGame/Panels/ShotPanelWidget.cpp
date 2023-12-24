#include "ShotPanelWidget.h"
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
	check(0 <= NewNum && NewNum <= 3);
	BulletsNum = NewNum;

	const auto Bullets = &Bullet1;

	for (int32 i = 0; i < NewNum; ++i)
		if (Bullets[i])
			Bullets[i]->SetVisibility(ESlateVisibility::Hidden);

	for (int32 i = NewNum; i < 3; ++i)
		if (Bullets[i])
			Bullets[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
