#pragma once

#include "Blueprint/UserWidget.h"
#include "TextWidget.generated.h"

class UTextBlock;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UTextWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;

	void SetText(const FText& NewText);
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(DesignerRebuild))
	FText Text;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> InternalTextBlock;
};
