#pragma once

#include "Blueprint/UserWidget.h"
#include "ScorePanelWidget.generated.h"

class UTextBlock;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UScorePanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	void SetScore(int32 NewScore) const;
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(ClampMin=0, ClampMax=999999, DesignerRebuild))
	int32 DefaultScore = 0;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
};
