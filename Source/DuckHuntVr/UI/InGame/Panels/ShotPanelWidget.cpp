#include "ShotPanelWidget.h"
#include "Components/Image.h"

void UShotPanelWidget::NativePreConstruct() {
	Super::NativePreConstruct();
	SetTheme(Theme);
	SetBulletsNum(BulletsNum);
}

void UShotPanelWidget::SetTheme(const ETheme NewTheme) {
	Theme = NewTheme;

	if (Background) {
		const auto BackgroundTextures = &BackgroundDuck;
		const auto NewBackgroundTexture = BackgroundTextures[static_cast<uint8>(NewTheme)].Get();
		Background->SetBrushFromTexture(NewBackgroundTexture);
	}

	const auto BulletTextures = &BulletDuck;
	const auto NewBulletTexture = BulletTextures[static_cast<uint8>(NewTheme)].Get();
	const auto Bullets = &Bullet1;
	for (int32 i = 0; i < 3; ++i)
		if (Bullets[i])
			Bullets[i]->SetBrushFromTexture(NewBulletTexture);
}

void UShotPanelWidget::SetBulletsNum(const int32 NewNum) {
	check(0 <= NewNum && NewNum <= 3);
	BulletsNum = NewNum;

	const auto Bullets = &Bullet1;

	for (int32 i = 0; i < NewNum; ++i)
		if (Bullets[i])
			Bullets[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	for (int32 i = NewNum; i < 3; ++i)
		if (Bullets[i])
			Bullets[i]->SetVisibility(ESlateVisibility::Hidden);
}
