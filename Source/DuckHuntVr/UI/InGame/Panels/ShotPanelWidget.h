#pragma once

#include "Blueprint/UserWidget.h"
#include "ShotPanelWidget.generated.h"

class UImage;
class UHorizontalBox;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UShotPanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void SynchronizeProperties() override;

	void SetSkyColor(const FLinearColor& NewColor);
	bool RemoveBullet();
	void ResetBullets();
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(sRGB="true", DesignerRebuild))
	FLinearColor SkyColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(ClampMin=0, ClampMax=3, DesignerRebuild))
	int32 DefaultBulletsNum = 3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> BackgroundSky;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> ImagesBox;
private:
	int32 BulletsNum = DefaultBulletsNum;
};
