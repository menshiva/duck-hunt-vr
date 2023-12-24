#pragma once

#include "Blueprint/UserWidget.h"
#include "DuckHuntVr/UI/InGame/Theme.h"
#include "HitPanelWidget.generated.h"

class UHorizontalBox;
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

	UPROPERTY(EditAnywhere, Category="Init|Stencil", DisplayName=Duck, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> DuckStencilTexture;

	UPROPERTY(EditAnywhere, Category="Init|Stencil", DisplayName=Clay, meta=(DesignerRebuild))
	TObjectPtr<UTexture2D> ClayStencilTexture;

	UPROPERTY(EditAnywhere, Category="Init|Colors", DisplayName=Miss, meta=(sRGB="true", DesignerRebuild))
	FLinearColor MissColor;

	UPROPERTY(EditAnywhere, Category="Init|Colors", DisplayName=Hit, meta=(sRGB="true", DesignerRebuild))
	FLinearColor HitColor;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> ImagesBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Stencil;
private:
	void UpdateHits() const;
};
