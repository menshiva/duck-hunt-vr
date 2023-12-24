#pragma once

#include "Blueprint/UserWidget.h"
#include "ShotPanelWidget.generated.h"

class UImage;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UShotPanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;

	void SetSkyColor(const FLinearColor& NewColor);
	void SetBulletsNum(int32 NewNum);
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(sRGB="true", DesignerRebuild))
	FLinearColor SkyColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(ClampMin=0, ClampMax=3, DesignerRebuild))
	int32 BulletsNum = 3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> BackgroundSky;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Bullet1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Bullet2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Bullet3;
};
