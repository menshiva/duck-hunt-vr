#include "ScorePanelWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UScorePanelWidget::NativePreConstruct() {
	Super::NativePreConstruct();
	SetScore(DefaultScore);
}

void UScorePanelWidget::SetScore(const int32 NewScore) const {
	if (ScoreText)
		ScoreText->SetText(UKismetTextLibrary::Conv_IntToText(NewScore, false, false, 6, 6));
}
