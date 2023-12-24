#include "HitPanelWidget.h"
#include "Components/Image.h"

UHitPanelWidget::UHitPanelWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	Hits.AddZeroed(10);
}

void UHitPanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetTheme(Theme);
}

void UHitPanelWidget::SetTheme(const ETheme NewTheme) {
	Theme = NewTheme;
	UpdateImages();
}

void UHitPanelWidget::SetHit(const int32 Idx, const bool Hit) {
	check(0 <= Idx && Idx < 10);
	Hits[Idx] = Hit;

	const auto ImagesToSet = &MissDuck;
	const auto Images = &Image1;
	const auto& ImageToSet = ImagesToSet[static_cast<uint8>(Hit) * 2 + static_cast<uint8>(Theme)];
	if (Images[Idx] && ImageToSet)
		Images[Idx]->SetBrushFromTexture(ImageToSet);
}

void UHitPanelWidget::ResetHits() {
	FMemory::Memzero(Hits.GetData(), Hits.Num() * sizeof(bool));
	UpdateImages();
}

void UHitPanelWidget::UpdateImages() {
	const auto ImagesToSet = &MissDuck;
	const auto Images = &Image1;
	for (int32 i = 0; i < 10; ++i) {
		const auto& ImageToSet = ImagesToSet[static_cast<uint8>(Hits[i]) * 2 + static_cast<uint8>(Theme)];
		if (Images[i] && ImageToSet)
			Images[i]->SetBrushFromTexture(ImageToSet);
	}
}
