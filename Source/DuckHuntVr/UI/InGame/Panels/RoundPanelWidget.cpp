#include "RoundPanelWidget.h"
#include "DuckHuntVr/UI/Widgets/TextWidget.h"
#include "Kismet/KismetTextLibrary.h"

void URoundPanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetRound(Round);
}

void URoundPanelWidget::SetRound(const int32 NewRound) {
	Round = NewRound;
	if (RoundText) {
		const auto RoundTextString = FString(TEXT("R=")) +
			UKismetTextLibrary::Conv_IntToText(NewRound, false, false, 1, 2).ToString();
		RoundText->SetText(FText::FromString(RoundTextString));
	}
}
