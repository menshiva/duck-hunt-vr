#include "HitPanelWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

UHitPanelWidget::UHitPanelWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	Hits.AddZeroed(10);
}

void UHitPanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetTheme(Theme);
	UpdateHits();
}

void UHitPanelWidget::SetTheme(const ETheme NewTheme) {
	Theme = NewTheme;
	if (Stencil) {
		const auto& StencilTextures = &DuckStencilTexture;
		Stencil->SetBrushFromTexture(StencilTextures[static_cast<uint8>(Theme)]);
	}
}

void UHitPanelWidget::SetHit(const int32 Idx, const bool Hit) {
	check(0 <= Idx && Idx < Hits.Num());
	Hits[Idx] = Hit;

	if (ImagesBox) {
		const auto& Slots = ImagesBox->GetSlots();
		if (Idx < Slots.Num()) {
			const auto& Colors = &MissColor;
			if (const auto Image = Cast<UImage>(Slots[Idx]->Content))
				Image->SetColorAndOpacity(Colors[Hit]);
		}
	}
}

void UHitPanelWidget::ResetHits() {
	FMemory::Memzero(Hits.GetData(), Hits.Num() * sizeof(bool));
	UpdateHits();
}

void UHitPanelWidget::UpdateHits() const {
	if (ImagesBox) {
		const auto& Colors = &MissColor;

		const auto& Slots = ImagesBox->GetSlots();
		check(Slots.Num() <= Hits.Num());

		for (int32 i = 0; i < Slots.Num(); ++i)
			if (const auto Image = Cast<UImage>(Slots[i]->Content))
				Image->SetColorAndOpacity(Colors[Hits[i]]);
	}
}
