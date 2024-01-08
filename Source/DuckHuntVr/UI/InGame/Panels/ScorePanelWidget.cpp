#include "ScorePanelWidget.h"
#include "DuckHuntVr/UI/Widgets/TextWidget.h"
#include "Kismet/KismetTextLibrary.h"

void UScorePanelWidget::SynchronizeProperties() {
	Super::SynchronizeProperties();
	SetScore(Score);
}

void UScorePanelWidget::SetScore(const int32 NewScore) {
	Score = NewScore;
	if (ScoreText)
		ScoreText->SetText(UKismetTextLibrary::Conv_IntToText(NewScore, false, false, 6, 6));
}
