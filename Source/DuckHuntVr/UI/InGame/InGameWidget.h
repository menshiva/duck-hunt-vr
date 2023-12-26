#pragma once

#include "Blueprint/UserWidget.h"
#include "Panels/HitPanelWidget.h"
#include "Panels/RoundPanelWidget.h"
#include "Panels/ScorePanelWidget.h"
#include "Panels/ShotPanelWidget.h"
#include "InGameWidget.generated.h"

UENUM(BlueprintType)
enum class EStateInfo : uint8 {
	None,
	FlyAway,
	// TODO: Pause
};

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UInGameWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;

	void SetStateInfo(EStateInfo NewStateInfo);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<URoundPanelWidget> RoundPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UShotPanelWidget> ShotPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHitPanelWidget> HitPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScorePanelWidget> ScorePanel;
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(DesignerRebuild))
	EStateInfo StateInfo = EStateInfo::None;

	UPROPERTY(EditAnywhere, Category="Init|States", DisplayName=FlyAway, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> FlyAwayStateTexture;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> StateInfoImage;
};
