#include "RoundPanelWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void URoundPanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetRound(Round);
}

void URoundPanelWidget::SetRound(const int32 NewRound) {
	Round = NewRound;
	if (RoundText)
		RoundText->SetText(UKismetTextLibrary::Conv_IntToText(NewRound, false, false, 1, 2));
}
