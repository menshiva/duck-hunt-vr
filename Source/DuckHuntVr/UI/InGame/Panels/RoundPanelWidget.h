#pragma once

#include "Blueprint/UserWidget.h"
#include "RoundPanelWidget.generated.h"

class UTextBlock;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API URoundPanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	void SetRound(int32 NewRound);
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(ClampMin=1, ClampMax=99, DesignerRebuild))
	int32 Round = 1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RoundText;
};
