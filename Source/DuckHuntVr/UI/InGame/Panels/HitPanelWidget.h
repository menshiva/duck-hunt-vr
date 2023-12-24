#pragma once

#include "Blueprint/UserWidget.h"
#include "DuckHuntVr/UI/InGame/Theme.h"
#include "HitPanelWidget.generated.h"

class UImage;

UCLASS(Abstract, Blueprintable, NotBlueprintType, meta=(DisableNativeTick))
class DUCKHUNTVR_API UHitPanelWidget : public UUserWidget {
	GENERATED_BODY()
public:
	explicit UHitPanelWidget(const FObjectInitializer& ObjectInitializer);

	virtual void SynchronizeProperties() override;

	void SetTheme(ETheme NewTheme);
	void SetHit(int32 Idx, bool Hit);
	void ResetHits();
protected:
	UPROPERTY(EditAnywhere, Category="Init|Properties", meta=(DesignerRebuild))
	ETheme Theme = ETheme::Duck;

	UPROPERTY(EditAnywhere, EditFixedSize, Category="Init|Properties", meta=(DesignerRebuild))
	TArray<bool> Hits;

	UPROPERTY(EditDefaultsOnly, Category="Init|Miss", DisplayName=Duck, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> MissDuck;

	UPROPERTY(EditDefaultsOnly, Category="Init|Miss", DisplayName=Clay, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> MissClay;

	UPROPERTY(EditDefaultsOnly, Category="Init|Hit", DisplayName=Duck, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> HitDuck;

	UPROPERTY(EditDefaultsOnly, Category="Init|Hit", DisplayName=Clay, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> HitClay;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image2;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image3;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image4;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image5;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image6;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image7;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image8;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image9;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image10;
private:
	void UpdateImages();
};
