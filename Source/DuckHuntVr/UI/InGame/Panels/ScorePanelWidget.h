#pragma once

#include "Blueprint/UserWidget.h"
#include "ScorePanelWidget.generated.h"

class UTextWidget;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UScorePanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;

	void SetScore(int32 NewScore);
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(ClampMin=0, ClampMax=999999, DesignerRebuild))
	int32 Score = 0;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextWidget> ScoreText;
};
