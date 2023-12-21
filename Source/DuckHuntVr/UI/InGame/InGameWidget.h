#pragma once

#include "Blueprint/UserWidget.h"
#include "Panels/RoundPanelWidget.h"
#include "Panels/ScorePanelWidget.h"
#include "Panels/ShotPanelWidget.h"
#include "InGameWidget.generated.h"

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UInGameWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	void SetTheme(ETheme NewTheme) const;

	FORCEINLINE URoundPanelWidget* GetRoundPanel() const { return RoundPanel; }
	FORCEINLINE UShotPanelWidget* GetShotPanel() const { return ShotPanel; }
	FORCEINLINE UScorePanelWidget* GetScorePanel() const { return ScorePanel; }
protected:
	UPROPERTY(EditDefaultsOnly, Category="Init|Properties", meta=(DesignerRebuild))
	ETheme DefaultTheme = ETheme::Duck;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URoundPanelWidget> RoundPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UShotPanelWidget> ShotPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScorePanelWidget> ScorePanel;
};
