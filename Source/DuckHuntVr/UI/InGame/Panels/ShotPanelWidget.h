#pragma once

#include "Blueprint/UserWidget.h"
#include "DuckHuntVr/UI/InGame/Theme.h"
#include "ShotPanelWidget.generated.h"

class UImage;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UShotPanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;

	void SetTheme(ETheme NewTheme);
	void SetBulletsNum(int32 NewNum);
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(DesignerRebuild))
	ETheme Theme = ETheme::Duck;

	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(ClampMin=0, ClampMax=3, DesignerRebuild))
	int32 BulletsNum = 3;

	UPROPERTY(EditDefaultsOnly, Category="Init|Background", DisplayName=Duck, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> BackgroundDuck;

	UPROPERTY(EditDefaultsOnly, Category="Init|Background", DisplayName=Clay, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> BackgroundClay;

	UPROPERTY(EditDefaultsOnly, Category="Init|Bullet", DisplayName=Duck, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> BulletDuck;

	UPROPERTY(EditDefaultsOnly, Category="Init|Bullet", DisplayName=Clay, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> BulletClay;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Bullet1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Bullet2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Bullet3;
};
