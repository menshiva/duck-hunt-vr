#include "TextWidget.h"
#include "Components/TextBlock.h"

void UTextWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetText(Text);
}

void UTextWidget::SetText(const FText& NewText) {
	Text = NewText;
	if (InternalTextBlock)
		InternalTextBlock->SetText(Text);
}
