#include "RoundPanelWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void URoundPanelWidget::NativePreConstruct() {
	Super::NativePreConstruct();
	SetRound(DefaultRound);
}

void URoundPanelWidget::SetRound(const int32 NewRound) const {
	if (RoundText)
		RoundText->SetText(UKismetTextLibrary::Conv_IntToText(NewRound, false, false, 1, 2));
}
