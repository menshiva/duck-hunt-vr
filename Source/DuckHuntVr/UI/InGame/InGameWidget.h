#pragma once

#include "Theme.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class URoundPanelWidget;
class UShotPanelWidget;
class UHitPanelWidget;
class UScorePanelWidget;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UInGameWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	void SetTheme(ETheme NewTheme);
protected:
	UPROPERTY(EditDefaultsOnly, Category="Init|Properties", meta=(DesignerRebuild))
	ETheme Theme = ETheme::Duck;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URoundPanelWidget> RoundPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UShotPanelWidget> ShotPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHitPanelWidget> HitPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScorePanelWidget> ScorePanel;
};
