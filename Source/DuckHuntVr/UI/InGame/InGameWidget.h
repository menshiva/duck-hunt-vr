#pragma once

#include "Blueprint/UserWidget.h"
#include "Panels/HitPanelWidget.h"
#include "Panels/RoundPanelWidget.h"
#include "Panels/ScorePanelWidget.h"
#include "Panels/ShotPanelWidget.h"
#include "InGameWidget.generated.h"

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UInGameWidget : public UUserWidget {
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URoundPanelWidget> RoundPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UShotPanelWidget> ShotPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHitPanelWidget> HitPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScorePanelWidget> ScorePanel;
};
