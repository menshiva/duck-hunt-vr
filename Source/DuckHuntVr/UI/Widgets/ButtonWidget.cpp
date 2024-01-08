#include "ButtonWidget.h"
#include "TextWidget.h"
#include "Components/Image.h"

void UButtonWidget::NativeConstruct() {
	Super::NativeConstruct();
	if (InternalButton) {
		InternalButton->OnHovered.AddDynamic(this, &UButtonWidget::OnInternalButtonHovered);
		InternalButton->OnUnhovered.AddDynamic(this, &UButtonWidget::OnInternalButtonUnhovered);
	}
}

void UButtonWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	if (Text) {
		Text->SetText(DefaultText);
		Text->SetColorAndOpacity(DefaultTextColor);
	}
}

void UButtonWidget::SetOnClickEvent(const FOnButtonClickedEvent& OnClickEvent) const {
	if (InternalButton)
		InternalButton->OnClicked = OnClickEvent;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UButtonWidget::OnInternalButtonHovered() {
	if (CursorImage)
		CursorImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UButtonWidget::OnInternalButtonUnhovered() {
	if (CursorImage)
		CursorImage->SetVisibility(ESlateVisibility::Hidden);
}
